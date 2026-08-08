/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 16:12:48 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 by lebeyssa                ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>

typedef struct s_codeur	t_codeur;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long long		available_at;
	int				is_locked;
}	t_dongle;


typedef struct s_queue
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				nb_coders;
	int				*coder_ids;
	int				count;
}	t_queue;


typedef struct s_finish
{
	pthread_mutex_t	finish_lock;
	int				finished;
	int				nb_compiles_done;
}	t_finish;


typedef struct s_print
{
	pthread_mutex_t	print_lock;
}	t_print;


typedef struct s_stop
{
	pthread_mutex_t	stop_lock;
	int				bool_stop;
}	t_stop;

typedef struct t_data
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
	char	sched_type;   

	t_stop			*stop;
	t_dongle		*dongles;
	t_codeur		*codeurs;
	t_queue			queue;
	long long		start_time;
}	t_data;

typedef struct s_codeur
{
	int				id;
	pthread_mutex_t	codeur_lock;
	t_dongle		*right_dongle;
	t_dongle		*left_dongle;
	int				has_dongles;
	t_data			*data;
	t_print			*print;
	t_stop			*stop;
	t_finish		*finish;
	long long		start_time;
	long long		last_meal;
	long long		last_compile;
	int				nb_compile;
	int				granted;
}	t_codeur;

typedef struct s_monitor_var
{
	t_data		*data;
	t_codeur	*codeur;
	int			nb_codeur;
	t_print		*print;
	t_stop		*stop;
}	t_monitor_var;

typedef struct s_simu
{
	pthread_t		*threads;
	pthread_t		t2;
	pthread_t		ticker;
	t_codeur		*codeur;
	t_dongle		*mutex;
	t_print			print;
	t_stop			stop_var;
	t_finish		*finish;
	t_data			*data;
	t_monitor_var	monitor;
}	t_simu;




int			parse_data(char **datav, t_data *data);
void		code_error_parsing(int error);
void		code_error_simulation(int error);
int			ft_isdigit(int c);
int			init_data(char **datav, t_data *data);
long long	ft_atoi(char *nptr);


int			init_dongle_mutex(t_simu *var_simu, t_data *data);
int			init_queue(t_data *data);
int			init_mutex_s_p_f(t_simu *var_simu);
int			init_monitor(t_data *data, t_simu *var_simu);
int			init_codeur(t_data *data, t_simu *var_simu);
int			create_thread(t_data *data, t_simu *var_simu);


void		join_all_threads(t_simu *var_simu, int y, int val);
void		join_threads(t_data *data, t_simu *var_simu);
int			manage_simu(t_data *data, int error);


void		free_all(t_simu *var_simu, int i);
void		destroy_cond(t_simu *var_simu, int y);
void		destroy_all_mutex(t_simu *var_simu, int y);
void		destroy_mutex(t_simu *var_simu);
void		free_alloc(t_simu *var_simu, int i);
int			destroy_free(t_simu *var_simu, int i, int error);
int			destroy_free_all(t_simu *var_simu, int i, int val);
int			destroy_free_all_mut(t_simu *var_simu, int i, int val);


void		finish_mutex(t_codeur *codeur);
void		*pthread_codeur(void *data);


void		queue_push(t_data *data, t_codeur *c);
void		queue_remove(t_data *data, int coder_id);
int			is_priority(t_data *data, int coder_id);
int			check_stop(t_data *data);
int			take_dongles(t_codeur *c);
void		release_dongles(t_codeur *c);


int			manage_codeur(t_codeur *codeur, int i, t_dongle *first,
				t_dongle *second);


int			check_stop_release(t_codeur *codeur, int release);
int			check_stop_display(t_codeur *codeur, char *str, int release);
int			check_sleep_release(t_codeur *codeur);


int			print_status(t_codeur *c, char *msg);


long long	get_timestamp_ms(void);
int			interruptible_sleep(t_codeur *c, long long ms);
long long	get_time_to_burnout(t_monitor_var *var);
long long	get_last_compile(t_codeur *c);
void		update_last_compile(t_codeur *c);


void		*pthread_moniteur(void *data);

void		*queue_ticker(void *arg);

#endif
