/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codeur.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:03:42 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 14:32:08 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	one_codeur(t_codeur *codeur)
{
	t_data	*data;

	if (codeur->left_dongle != codeur->right_dongle)
		return (0);
	data = codeur->data;
	pthread_mutex_lock(&data->queue.lock);
	codeur->left_dongle->is_locked = 1;
	pthread_mutex_unlock(&data->queue.lock);
	print_status(codeur, " has taken a dongle\n");
	while (check_stop(data) == 0)
		usleep(1000);
	pthread_mutex_lock(&data->queue.lock);
	codeur->left_dongle->is_locked = 0;
	pthread_mutex_unlock(&data->queue.lock);
	return (-1);
}

void	finish_mutex(t_codeur *codeur)
{
	pthread_mutex_lock(&codeur->finish->finish_lock);
	codeur->finish->finished = 1;
	pthread_mutex_unlock(&codeur->finish->finish_lock);
}

void	*pthread_codeur(void *data)
{
	t_codeur	*codeur;
	int			i;

	codeur = (t_codeur *)data;
	if (codeur->id % 2 == 0)
		usleep(1000);
	if (one_codeur(codeur))
	{
		finish_mutex(codeur);
		return (NULL);
	}
	i = 0;
	if (manage_codeur(codeur, i))
		return (NULL);
	finish_mutex(codeur);
	return (NULL);
}
