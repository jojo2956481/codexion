/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_ticker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:30:52 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/08 16:44:43 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*queue_ticker(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!check_stop(data))
	{
		usleep(2000);
		pthread_mutex_lock(&data->queue.lock);
		pthread_cond_broadcast(&data->queue.cond);
		pthread_mutex_unlock(&data->queue.lock);
	}
	return (NULL);
}