/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:23:28 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_timestamp_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	interruptible_sleep(t_codeur *c, long long ms)
{
	long long	end;

	end = get_timestamp_ms() + ms;
	while (get_timestamp_ms() < end)
	{
		pthread_mutex_lock(&c->stop->stop_lock);
		if (c->stop->bool_stop)
		{
			pthread_mutex_unlock(&c->stop->stop_lock);
			return (1);
		}
		pthread_mutex_unlock(&c->stop->stop_lock);
		usleep(1000);
	}
	return (0);
}

long long	get_time_to_burnout(t_monitor_var *var)
{
	long long	t;

	pthread_mutex_lock(&var->stop->stop_lock);
	t = var->data->time_to_burnout;
	pthread_mutex_unlock(&var->stop->stop_lock);
	return (t);
}
