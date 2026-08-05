/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:47:41 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_stop_release(t_dongle *first, t_dongle *second,
	t_codeur *codeur, int release)
{
	pthread_mutex_lock(&codeur->stop->stop_lock);
	if (codeur->stop->bool_stop == 1)
	{
		if (release)
		{
			if (codeur->left_locked == 1)
			{
				release_dongle(first, codeur->data->dongle_cooldown);
				codeur->left_locked = 0;
			}
			if (codeur->right_locked == 1)
			{
				release_dongle(second, codeur->data->dongle_cooldown);
				codeur->right_locked = 0;
			}
		}
		pthread_mutex_unlock(&codeur->stop->stop_lock);
		return (-1);
	}
	pthread_mutex_unlock(&codeur->stop->stop_lock);
	return (0);
}

int	check_stop_display(t_codeur *codeur, char *str, int release)
{
	if (release)
		update_last_compile(codeur);
	if (print_status(codeur, str))
	{
		if (release)
		{
			if (codeur->left_locked == 1)
			{
				release_dongle(codeur->left_dongle,
					codeur->data->dongle_cooldown);
				codeur->left_locked = 0;
			}
			if (codeur->right_locked == 1)
			{
				release_dongle(codeur->right_dongle,
					codeur->data->dongle_cooldown);
				codeur->right_locked = 0;
			}
		}
		return (-1);
	}
	return (0);
}

int	check_sleep_release(t_codeur *codeur, t_dongle *first, t_dongle *second)
{
	if (interruptible_sleep(codeur, codeur->data->time_to_compile))
	{
		if (codeur->left_locked == 1)
		{
			release_dongle(codeur->left_dongle, codeur->data->dongle_cooldown);
			codeur->left_locked = 0;
		}
		if (codeur->right_locked == 1)
		{
			release_dongle(codeur->right_dongle, codeur->data->dongle_cooldown);
			codeur->right_locked = 0;
		}
		return (-1);
	}
	release_dongle(first, codeur->data->dongle_cooldown);
	codeur->left_locked = 0;
	release_dongle(second, codeur->data->dongle_cooldown);
	codeur->right_locked = 0;
	pthread_mutex_lock(&codeur->finish->finish_lock);
	codeur->finish->nb_compiles_done += 1;
	pthread_mutex_unlock(&codeur->finish->finish_lock);
	return (0);
}
