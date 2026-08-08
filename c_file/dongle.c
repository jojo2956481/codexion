/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:23:07 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 by lebeyssa                ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	push_fifo(t_data *data, t_codeur *c, int count)
{
	data->queue.coder_ids[count] = c->id;
}


static void	push_lifo(t_data *data, t_codeur *c, int count)
{
	int	i;

	i = count;
	while (i > 0)
	{
		data->queue.coder_ids[i] = data->queue.coder_ids[i - 1];
		i--;
	}
	data->queue.coder_ids[0] = c->id;
}


static void	push_edf(t_data *data, t_codeur *c, int count)
{
	int			pos;
	int			prev_id;
	t_codeur	*prev;

	pos = count;
	while (pos > 0)
	{
		prev_id = data->queue.coder_ids[pos - 1];
		prev = &data->codeurs[prev_id - 1];
		if (c->last_meal < prev->last_meal)
		{
			data->queue.coder_ids[pos] = prev_id;
			pos--;
		}
		else
			break ;
	}
	data->queue.coder_ids[pos] = c->id;
}

void	queue_push(t_data *data, t_codeur *c)
{
	int	count;

	count = data->queue.count;
	if (data->sched_type == 'f')
		push_fifo(data, c, count);
	else if (data->sched_type == 'l')
		push_lifo(data, c, count);
	else if (data->sched_type == 'e')
		push_edf(data, c, count);
	data->queue.count++;
	pthread_cond_broadcast(&data->queue.cond);
}

static void	queue_remove_locked(t_data *data, int coder_id)
{
	int	index;
	int	i;

	index = -1;
	i = 0;
	while (i < data->queue.count)
	{
		if (data->queue.coder_ids[i] == coder_id)
		{
			index = i;
			break ;
		}
		i++;
	}
	if (index != -1)
	{
		i = index;
		while (i < data->queue.count - 1)
		{
			data->queue.coder_ids[i] = data->queue.coder_ids[i + 1];
			i++;
		}
		data->queue.count--;
	}
}

void	queue_remove(t_data *data, int coder_id)
{
	pthread_mutex_lock(&data->queue.lock);
	queue_remove_locked(data, coder_id);
	pthread_mutex_unlock(&data->queue.lock);
}

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
	t_dongle	*e1;
	t_dongle	*e2;
	int			any;

	i = 0;
	any = 0;
	while (i < data->queue.count)
	{
		id = data->queue.coder_ids[i];
		cd = &data->codeurs[id - 1];
		e1 = (cd->left_dongle < cd->right_dongle)
			? cd->left_dongle : cd->right_dongle;
		e2 = (cd->left_dongle < cd->right_dongle)
			? cd->right_dongle : cd->left_dongle;
		if (!e1->is_locked && now >= e1->available_at
			&& !e2->is_locked && now >= e2->available_at)
		{
			e1->is_locked = 1;
			e2->is_locked = 1;
			cd->granted = 1;
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
		{
			queue_remove_locked(data, c->id);
			pthread_mutex_unlock(&data->queue.lock);
			return (-1);
		}
		if (c->granted)
		{
			c->granted = 0;
			c->has_dongles = 1;
			pthread_mutex_unlock(&data->queue.lock);
			print_status(c, " has taken a dongle\n");
			print_status(c, " has taken a dongle\n");
			return (0);
		}
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
