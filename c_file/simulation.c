/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 13:10:44 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	join_all_threads(t_simu	*var_simu, int y, int val)
{
	int	i;

	i = 0;
	if (y == -1)
		y = var_simu->data->number_of_coders;
	if (val == 1)
	{
		while (i < y)
		{
			pthread_join(var_simu->threads[i], NULL);
			var_simu->threads[i] = 0;
			i++;
		}
		{
			pthread_join(var_simu->t2, NULL);
			var_simu->t2 = 0;
		}
	}
}

void	join_threads(t_data *data, t_simu *var_simu)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(var_simu->threads[i], NULL);
		i++;
	}
	pthread_join(var_simu->t2, NULL);
}

static int	alloc(t_data *data, t_simu *var_simu)
{
	var_simu->finish = NULL;
	var_simu->threads = malloc(sizeof(pthread_t) * data->number_of_coders);
	if (!var_simu->threads)
		return (-2);
	var_simu->codeur = malloc(sizeof(t_codeur) * data->number_of_coders);
	if (!var_simu->codeur)
	{
		free(var_simu->threads);
		var_simu->threads = NULL;
		return (-2);
	}
	data->codeurs = var_simu->codeur;
	var_simu->mutex = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!var_simu->mutex)
	{
		free(var_simu->codeur);
		var_simu->codeur = NULL;
		free(var_simu->threads);
		var_simu->threads = NULL;
		return (-2);
	}
	return (0);
}

static void	finish_simu(t_data *data, t_simu *var_simu)
{
	join_threads(data, var_simu);
	destroy_cond(var_simu, -1);
	destroy_all_mutex(var_simu, -1);
	destroy_mutex(var_simu);
	free_alloc(var_simu, -1);
}

int	manage_simu(t_data *data, int error)
{
	t_simu	var_simu;

	var_simu.data = data;
	if (alloc(data, &var_simu))
		return (-2);
	error = init_dongle_mutex(&var_simu, data);
	if (error < 0)
		return (error);
	if (init_mutex_s_p_f(&var_simu) != 0)
		return (-1);
	error = init_monitor(data, &var_simu);
	if (error < 0)
		return (error);
	error = init_codeur(data, &var_simu);
	if (error < 0)
		return (error);
	finish_simu(data, &var_simu);
	return (1);
}
