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


static void	wait_until_ms(pthread_cond_t *cond, pthread_mutex_t *mutex, int ms)
{
	struct timespec	ts;
	long long		wake_ms;

	if (ms < 1)
		ms = 1;
	clock_gettime(CLOCK_REALTIME, &ts);
	wake_ms = (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000 + ms;
	ts.tv_sec = wake_ms / 1000;
	ts.tv_nsec = (wake_ms % 1000) * 1000000;
	pthread_cond_timedwait(cond, mutex, &ts);
}

/* FIFO : le nouveau va en fin de file. */
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

	pthread_mutex_lock(&data->queue.lock);
	count = data->queue.count;
	if (data->sched_type == 'f')
		push_fifo(data, c, count);
	else if (data->sched_type == 'l')
		push_lifo(data, c, count);
	else if (data->sched_type == 'e')
		push_edf(data, c, count);
	data->queue.count++;
	pthread_cond_broadcast(&data->queue.cond);
	pthread_mutex_unlock(&data->queue.lock);
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

static long long	compute_wait_target(t_dongle *d1, t_dongle *d2, long long now)
{
	long long	target;

	if (d1->is_locked || d2->is_locked)
		return (now + 5);
	target = d1->available_at;
	if (d2->available_at > target)
		target = d2->available_at;
	if (target <= now)
		return (now + 5);
	return (target);
}

int	take_dongles(t_codeur *c)
{
	t_data		*data;
	t_dongle	*d1;
	t_dongle	*d2;
	long long	now;

	data = c->data;
	queue_push(data, c);
	pthread_mutex_lock(&data->queue.lock);
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
			return (0);
		}
		now = get_timestamp_ms();
		grant_all_runnable(data, now);
		if (c->granted)
			continue ;
		d1 = (c->left_dongle < c->right_dongle) ? c->left_dongle : c->right_dongle;
		d2 = (c->left_dongle < c->right_dongle) ? c->right_dongle : c->left_dongle;
		wait_until_ms(&data->queue.cond, &data->queue.lock,
			(int)(compute_wait_target(d1, d2, now) - now));
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
