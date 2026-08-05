/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:23:18 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_finished(t_codeur *c, int id)
{
	int	ret;

	pthread_mutex_lock(&c->finish->finish_lock);
	ret = c->all_codeurs[id].finish->finished;
	pthread_mutex_unlock(&c->finish->finish_lock);
	return (ret);
}

static int	is_stopped(t_codeur *c, int id)
{
	int	ret;

	pthread_mutex_lock(&c->stop->stop_lock);
	ret = c->all_codeurs[id].stop->bool_stop;
	pthread_mutex_unlock(&c->stop->stop_lock);
	return (ret);
}

static int	calcul_priority(t_codeur *c, long long my_deadline, t_dongle *d)
{
	long long	other_deadline;
	int			other_id;
	int			id;
	int			i;

	i = 0;
	id = c->id -1;
	while (i < d->waiting_count)
	{
		other_id = d->waiting[i];
		if (other_id != id && !is_finished(c, other_id)
			&& !is_stopped(c, other_id))
		{
			other_deadline = get_last_compile(&c->all_codeurs[other_id])
				+ c->all_codeurs[other_id].data->time_to_burnout;
			if (other_deadline < my_deadline)
				return (0);
		}
		i++;
	}
	return (1);
}

int	i_have_priority_edf(t_codeur *c, t_dongle *d)
{
	long long	my_deadline;

	my_deadline = get_last_compile(c)
		+ c->data->time_to_burnout;
	if (calcul_priority(c, my_deadline, d) == 0)
		return (0);
	return (1);
}
