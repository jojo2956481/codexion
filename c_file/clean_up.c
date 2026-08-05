/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:23:02 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all(t_simu *var_simu, int i)
{
	while (i > 0)
	{
		i--;
		free(var_simu->mutex[i].waiting);
	}
}

void	destroy_cond(t_simu *var_simu, int y)
{
	int	i;

	if (y == -1)
		y = var_simu->data->number_of_coders;
	i = 0;
	while (i < y)
	{
		pthread_cond_destroy(&var_simu->mutex[i].cond);
		i++;
	}
}

void	destroy_all_mutex(t_simu *var_simu, int y)
{
	int	i;

	if (y == -1)
		y = var_simu->data->number_of_coders;
	i = 0;
	while (i < y)
	{
		pthread_mutex_destroy(&var_simu->mutex[i].mutex);
		i++;
	}
}

void	destroy_mutex(t_simu *var_simu)
{
	pthread_mutex_destroy(&var_simu->print.print_lock);
	pthread_mutex_destroy(&var_simu->stop_var.stop_lock);
	pthread_mutex_destroy(&var_simu->finish.finish_lock);
}

void	free_alloc(t_simu *var_simu, int i)
{
	if (i == -1)
		i = var_simu->data->number_of_coders;
	free_all(var_simu, i);
	free(var_simu->threads);
	free(var_simu->codeur);
	free(var_simu->mutex);
}
