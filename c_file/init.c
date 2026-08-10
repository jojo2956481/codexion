/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:02:28 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 14:25:07 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongle_mutex(t_simu *var_simu, t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->number_of_coders)
	{
		if (pthread_mutex_init(&var_simu->mutex[i].mutex, NULL))
			return (destroy_free(var_simu, i, -1));
		var_simu->mutex[i].available_at = 0;
		var_simu->mutex[i].is_locked = 0;
	}
	return (0);
}

int	init_queue(t_simu *var_simu)
{
	t_data	*data;

	data = var_simu->data;
	if (pthread_mutex_init(&data->queue.lock, NULL))
		return (free_alloc(var_simu, -1, -1));
	if (pthread_cond_init(&data->queue.cond, NULL))
		return (free_alloc(var_simu, -1, -1));
	data->queue.coder_ids = malloc(sizeof(int) * data->number_of_coders);
	if (!data->queue.coder_ids)
		return (free_alloc(var_simu, -1, -2));
	data->queue.nb_coders = data->number_of_coders;
	data->queue.count = 0;
	return (0);
}

int	init_mutex_s_p_f(t_simu *var_simu, int error)
{
	int	i;

	if (pthread_mutex_init(&var_simu->stop_var.stop_lock, NULL))
		return (free_alloc(var_simu, -1, -1));
	var_simu->stop_var.bool_stop = 0;
	if (pthread_mutex_init(&var_simu->print.print_lock, NULL))
		return (free_alloc(var_simu, -1, -1));
	var_simu->finish = malloc(sizeof(t_finish)
			* var_simu->data->number_of_coders);
	if (!var_simu->finish)
		return (free_alloc(var_simu, -1, -2));
	i = -1;
	while (++i < var_simu->data->number_of_coders)
	{
		if (pthread_mutex_init(&var_simu->finish[i].finish_lock, NULL))
			return (free_alloc(var_simu, -1, -1));
		var_simu->finish[i].finished = 0;
		var_simu->finish[i].nb_compiles_done = 0;
	}
	var_simu->data->stop = &var_simu->stop_var;
	error = init_queue(var_simu);
	if (error)
		return (error);
	return (0);
}

int	init_monitor(t_data *data, t_simu *var_simu)
{
	int	nb_codeur;

	nb_codeur = data->number_of_coders;
	var_simu->monitor.data = data;
	var_simu->monitor.codeur = var_simu->codeur;
	var_simu->monitor.nb_codeur = nb_codeur;
	var_simu->monitor.print = &var_simu->print;
	var_simu->monitor.stop = &var_simu->stop_var;
	return (0);
}

int	boucle_codeur(t_data *data, t_simu *var_simu,
		long long start_time, int i)
{
	while (++i < data->number_of_coders)
	{
		var_simu->codeur[i].id = i + 1;
		var_simu->codeur[i].right_dongle = &var_simu->mutex[i];
		var_simu->codeur[i].left_dongle = &var_simu->mutex[
			(i + 1) % data->number_of_coders];
		var_simu->codeur[i].has_dongles = 0;
		var_simu->codeur[i].granted = 0;
		var_simu->codeur[i].start_time = start_time;
		var_simu->codeur[i].print = &var_simu->print;
		var_simu->codeur[i].data = data;
		var_simu->codeur[i].stop = &var_simu->stop_var;
		var_simu->codeur[i].finish = &var_simu->finish[i];
		var_simu->codeur[i].last_compile = start_time;
		var_simu->codeur[i].last_meal = start_time;
		if (pthread_mutex_init(&var_simu->codeur[i].codeur_lock, NULL))
			return (destroy_free_all_mut(var_simu, i, -1));
	}
	return (create_thread(data, var_simu));
}
