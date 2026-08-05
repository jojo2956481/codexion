/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moniteur.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:23:15 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static	int	stop_and_display(t_monitor_var *var, int i)
{
	pthread_mutex_lock(&var->codeur[i].stop->stop_lock);
	var->codeur[i].stop->bool_stop = 1;
	pthread_mutex_unlock(&var->codeur[i].stop->stop_lock);
	pthread_mutex_lock(&var->codeur[i].print->print_lock);
	printf("%lld %d %s", get_timestamp_ms() - var->codeur[i].start_time,
		var->codeur[i].id, "burned out\n");
	pthread_mutex_unlock(&var->codeur[i].print->print_lock);
	return (1);
}

static int	check_stop_condition(t_monitor_var *var)
{
	int	i;
	int	all_done;

	i = -1;
	all_done = 1;
	while (++i < var->nb_codeur)
	{
		pthread_mutex_lock(&var->codeur[i].finish->finish_lock);
		if (var->codeur[i].finish->finished)
		{
			pthread_mutex_unlock(&var->codeur[i].finish->finish_lock);
			continue ;
		}
		pthread_mutex_unlock(&var->codeur[i].finish->finish_lock);
		all_done = 0;
		if (get_timestamp_ms() - get_last_compile(&var->codeur[i])
			> get_time_to_burnout(var))
		{
			return (stop_and_display(var, i));
		}
	}
	return (all_done);
}

void	*pthread_moniteur(void *data)
{
	int				stop;
	t_monitor_var	*var;

	var = (t_monitor_var *)data;
	stop = 0;
	while (!stop)
	{
		if (check_stop_condition(var))
			stop = 1;
		usleep(500);
	}
	return (NULL);
}
