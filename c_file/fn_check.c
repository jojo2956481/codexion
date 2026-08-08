/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:47:41 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 by lebeyssa                ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_stop_release(t_codeur *codeur, int release)
{
	int	stopped;

	pthread_mutex_lock(&codeur->stop->stop_lock);
	stopped = codeur->stop->bool_stop;
	pthread_mutex_unlock(&codeur->stop->stop_lock);
	if (stopped)
	{
		if (release)
			release_dongles(codeur);
		return (-1);
	}
	return (0);
}

int	check_stop_display(t_codeur *codeur, char *str, int release)
{
	if (release)
		update_last_compile(codeur);
	if (print_status(codeur, str))
	{
		if (release)
			release_dongles(codeur);
		return (-1);
	}
	return (0);
}

int	check_sleep_release(t_codeur *codeur)
{
	if (interruptible_sleep(codeur, codeur->data->time_to_compile))
	{
		release_dongles(codeur);
		return (-1);
	}
	release_dongles(codeur);
	pthread_mutex_lock(&codeur->finish->finish_lock);
	codeur->finish->nb_compiles_done += 1;
	pthread_mutex_unlock(&codeur->finish->finish_lock);
	return (0);
}
