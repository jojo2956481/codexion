/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:23:25 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/07/27 12:46:55 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	print_status(t_codeur *c, char *msg)
{
	pthread_mutex_lock(&c->stop->stop_lock);
	if (c->stop->bool_stop)
	{
		pthread_mutex_unlock(&c->stop->stop_lock);
		return (1);
	}
	pthread_mutex_unlock(&c->stop->stop_lock);
	pthread_mutex_lock(&c->print->print_lock);
	printf("%lld %d%s",
		get_timestamp_ms() - c->start_time, c->id, msg);
	pthread_mutex_unlock(&c->print->print_lock);
	return (0);
}
