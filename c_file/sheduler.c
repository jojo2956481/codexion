/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dongle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:04:39 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 13:44:15 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	push_fifo(t_data *data, t_codeur *c, int count)
{
	data->queue.coder_ids[count] = c->id;
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
	if (data->scheduler[0] == 'f')
		push_fifo(data, c, count);
	else if (data->scheduler[0] == 'e')
		push_edf(data, c, count);
	data->queue.count++;
	pthread_cond_broadcast(&data->queue.cond);
}

void	queue_remove_locked(t_data *data, int coder_id)
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
