/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:23:07 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void queue_push(t_data *data, t_codeur *c)
{
    
    int			pos:
	int			count;
	int			prec_id;
	t_codeur	*prev;
	
	pthread_mutex_lock(&data->queue.lock);
	count = data->queue.count;
    if (data->sched_type == 'f')
    {
        data->queue.coder_ids[count] = c->id;
    }
    else if (data->sched_type == 'e')
    {
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
                break;
        }
        data->queue.coder_ids[pos] = c->id;
    }
    
    data->queue.count++;
    pthread_cond_broadcast(&data->queue.cond);
    pthread_mutex_unlock(&data->queue.lock);
}


void queue_remove(t_data *data, int coder_id)
{
	int index;
	int	i;
	
    pthread_mutex_lock(&data->queue.lock);
    index = -1;
	i = 0;
    while (i < data->queue.count)
    {
        if (data->queue.coder_ids[i] == coder_id)
        {
            index = i;
            break;
        }
		i++;
    }
    if (index != -1)
    {
		i = index;
        while (i < data->queue.count - 1)
            data->queue.coder_ids[i] = data->queue.coder_ids[i + 1];
		i++
        data->queue.count--;
    }
    pthread_mutex_unlock(&data->queue.lock);
}

int is_priority(t_data *data, int coder_id)
{
	int	is_first
    is_first = 0;
    pthread_mutex_lock(&data->queue.lock);
    if (data->queue.count > 0 && data->queue.coder_ids[0] == coder_id)
        is_first = 1;
    pthread_mutex_unlock(&data->queue.lock);
    return (is_first);
}

void release_dongles(t_codeur *c)
{
	long long next_avail;
    t_data *data = c->data;
    next_avail = get_timestamp_ms() + data->dongle_cooldown;

    pthread_mutex_lock(&c->left->mutex);
    c->left->is_locked = 0;
    c->left->available_at = next_avail;
    pthread_mutex_unlock(&c->left->mutex);

    pthread_mutex_lock(&c->right->mutex);
    c->right->is_locked = 0;
    c->right->available_at = next_avail;
    pthread_mutex_unlock(&c->right->mutex);
    c->last_meal = get_timestamp_ms();
    pthread_mutex_lock(&data->queue.lock);
    pthread_cond_broadcast(&data->queue.cond);
    pthread_mutex_unlock(&data->queue.lock);
}
