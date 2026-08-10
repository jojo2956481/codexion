/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dongles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:29:23 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 14:38:27 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	order_dongles(t_codeur *cd, t_dongle **e1, t_dongle **e2)
{
	if (cd->left_dongle < cd->right_dongle)
	{
		*e1 = cd->left_dongle;
		*e2 = cd->right_dongle;
	}
	else
	{
		*e1 = cd->right_dongle;
		*e2 = cd->left_dongle;
	}
}

int	try_grant(t_codeur *cd, long long now)
{
	t_dongle	*e1;
	t_dongle	*e2;

	order_dongles(cd, &e1, &e2);
	if (e1->is_locked || now < e1->available_at)
		return (0);
	if (e2->is_locked || now < e2->available_at)
		return (0);
	e1->is_locked = 1;
	e2->is_locked = 1;
	cd->granted = 1;
	return (1);
}

int	handle_stop(t_data *data, t_codeur *c)
{
	queue_remove_locked(data, c->id);
	pthread_mutex_unlock(&data->queue.lock);
	return (-1);
}

int	handle_granted(t_data *data, t_codeur *c)
{
	c->granted = 0;
	c->has_dongles = 1;
	pthread_mutex_unlock(&data->queue.lock);
	print_status(c, " has taken a dongle\n");
	print_status(c, " has taken a dongle\n");
	return (0);
}
