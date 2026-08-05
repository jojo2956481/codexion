/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 16:12:48 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:24:08 by lebeyssa         ###   ########lyon.fr   */
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

typedef struct s_codeur	t_codeur;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long long		available_at;
	int				is_locked;
}	t_dongle;

typedef struct s_queue {
    pthread_mutex_t lock;
    pthread_cond_t  cond;
	int				nb_coders;
    int             *coder_ids;
    int             count;
} t_queue;

typedef struct s_finish
{
	pthread_mutex_t	finish_lock;
	int				finished;
	int				nb_compiles_done;
}	t_finish;


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

	int             stop;
    pthread_mutex_t stop_lock;
    pthread_mutex_t print_lock;
    
    t_dongle        *dongles;
    t_codeur        *codeurs;
    t_queue         queue;
    long long       start_time;
}	t_data;

typedef struct s_codeur
{
	int				id;
	pthread_mutex_t	codeur_lock;
	t_dongle		*right_dongle;
	t_dongle		*left_dongle;
	t_data			*data;
	long long		start_time;
	long long       last_meal;
	int				nb_compile;
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
	t_codeur		*codeur;
	t_dongle		*mutex;
	t_print			print;
	t_stop			stop_var;
	t_finish		finish;
	t_data			*data;
	t_monitor_var	monitor;
}	t_simu;

#endif