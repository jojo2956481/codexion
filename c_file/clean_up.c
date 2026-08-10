/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:01:38 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 13:01:55 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all(t_simu *var_simu, int i)
{
	(void)i;
	if (var_simu->data->queue.coder_ids)
	{
		pthread_mutex_destroy(&var_simu->data->queue.lock);
		pthread_cond_destroy(&var_simu->data->queue.cond);
		free(var_simu->data->queue.coder_ids);
		var_simu->data->queue.coder_ids = NULL;
	}
}

void	destroy_cond(t_simu *var_simu, int y)
{
	(void)var_simu;
	(void)y;
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
	int	i;

	pthread_mutex_destroy(&var_simu->print.print_lock);
	pthread_mutex_destroy(&var_simu->stop_var.stop_lock);
	if (var_simu->finish)
	{
		i = 0;
		while (i < var_simu->data->number_of_coders)
		{
			pthread_mutex_destroy(&var_simu->finish[i].finish_lock);
			i++;
		}
	}
}

int	free_alloc(t_simu *var_simu, int i, int error)
{
	if (i == -1)
		i = var_simu->data->number_of_coders;
	free_all(var_simu, i);
	free(var_simu->threads);
	free(var_simu->codeur);
	free(var_simu->mutex);
	if (var_simu->finish)
		free(var_simu->finish);
	return (error);
}
