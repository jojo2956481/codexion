/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:03:24 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 14:30:12 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_priority(t_data *data, int coder_id)
{
	int	is_first;

	is_first = 0;
	pthread_mutex_lock(&data->queue.lock);
	if (data->queue.count > 0 && data->queue.coder_ids[0] == coder_id)
		is_first = 1;
	pthread_mutex_unlock(&data->queue.lock);
	return (is_first);
}

int	check_stop(t_data *data)
{
	int	ret;

	pthread_mutex_lock(&data->stop->stop_lock);
	ret = data->stop->bool_stop;
	pthread_mutex_unlock(&data->stop->stop_lock);
	return (ret);
}

static void	grant_all_runnable(t_data *data, long long now)
{
	int			i;
	int			id;
	t_codeur	*cd;
	int			any;

	i = 0;
	any = 0;
	while (i < data->queue.count)
	{
		id = data->queue.coder_ids[i];
		cd = &data->codeurs[id - 1];
		if (try_grant(cd, now))
		{
			queue_remove_locked(data, id);
			any = 1;
		}
		else
			i++;
	}
	if (any)
		pthread_cond_broadcast(&data->queue.cond);
}

int	take_dongles(t_codeur *c)
{
	t_data		*data;
	long long	now;

	data = c->data;
	pthread_mutex_lock(&data->queue.lock);
	queue_push(data, c);
	while (1)
	{
		if (check_stop(data))
			return (handle_stop(data, c));
		if (c->granted)
			return (handle_granted(data, c));
		now = get_timestamp_ms();
		grant_all_runnable(data, now);
		if (c->granted)
			continue ;
		pthread_cond_wait(&data->queue.cond, &data->queue.lock);
	}
}

void	release_dongles(t_codeur *c)
{
	long long	next_avail;
	t_data		*data;

	if (!c->has_dongles)
		return ;
	data = c->data;
	pthread_mutex_lock(&data->queue.lock);
	next_avail = get_timestamp_ms() + data->dongle_cooldown;
	c->left_dongle->is_locked = 0;
	c->left_dongle->available_at = next_avail;
	c->right_dongle->is_locked = 0;
	c->right_dongle->available_at = next_avail;
	c->has_dongles = 0;
	c->last_meal = get_timestamp_ms();
	pthread_cond_broadcast(&data->queue.cond);
	pthread_mutex_unlock(&data->queue.lock);
}
