/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:02:06 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 13:33:31 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	destroy_free(t_simu *var_simu, int i, int error)
{
	destroy_cond(var_simu, i);
	destroy_all_mutex(var_simu, i);
	free_alloc(var_simu, i, 0);
	return (error);
}

int	destroy_free_all(t_simu *var_simu, int i, int val)
{
	int	j;

	j = -1;
	pthread_mutex_lock(&var_simu->stop_var.stop_lock);
	var_simu->stop_var.bool_stop = 1;
	pthread_mutex_unlock(&var_simu->stop_var.stop_lock);
	pthread_mutex_lock(&var_simu->data->queue.lock);
	pthread_cond_broadcast(&var_simu->data->queue.cond);
	pthread_mutex_unlock(&var_simu->data->queue.lock);
	join_all_threads(var_simu, i, val);
	destroy_cond(var_simu, -1);
	destroy_all_mutex(var_simu, -1);
	destroy_mutex(var_simu);
	j = -1;
	while (++j < i)
		pthread_mutex_destroy(&var_simu->codeur[j].codeur_lock);
	free_alloc(var_simu, -1, 0);
	return (-3);
}

int	destroy_free_all_mut(t_simu *var_simu, int i, int val)
{
	int	j;

	j = 0;
	destroy_cond(var_simu, 0);
	destroy_all_mutex(var_simu, 0);
	destroy_mutex(var_simu);
	while (j < i)
	{
		pthread_mutex_destroy(&var_simu->codeur[j].codeur_lock);
		j++;
	}
	free_alloc(var_simu, 0, 0);
	return (val);
}
