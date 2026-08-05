/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codeur.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:22:54 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	one_codeur(t_codeur *codeur)
{
	if (codeur->left_dongle == codeur->right_dongle)
	{
		wait_dongle(codeur->left_dongle, codeur);
		print_status(codeur, " has taken a dongle\n");
		while (check_stop_release(NULL, NULL, codeur, 0) == 0)
			usleep(1000);
		release_dongle(codeur->left_dongle,
			codeur->data->dongle_cooldown);
		return (-1);
	}
	return (0);
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
	t_dongle	*first;
	t_dongle	*second;
	int			i;

	codeur = (t_codeur *)data;
	if (codeur->id % 2 == 0)
		usleep(1000);
	first = codeur->left_dongle;
	second = codeur->right_dongle;
	if (one_codeur(codeur))
		return (NULL);
	i = 0;
	if (manage_codeur(codeur, i, first, second))
		return (NULL);
	finish_mutex(codeur);
	return (NULL);
}
