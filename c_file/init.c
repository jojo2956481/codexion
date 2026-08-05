/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:23:09 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 15:06:52 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongle_mutex(t_simu *var_simu, t_data *data)
{
	int	i;
	int	*stack;

	i = -1;
	while (++i < data->number_of_coders)
	{
		if (pthread_mutex_init(&var_simu->mutex[i].mutex, NULL))
			return (destroy_free(var_simu, i, -1));
		if (pthread_cond_init(&var_simu->mutex[i].cond, NULL))
			return (destroy_free(var_simu, i, -1));
		var_simu->mutex[i].available_at = 0;
		var_simu->mutex[i].is_locked = 0;
	}
	return (0);
}

int init_queue(t_data *data):
{
	if (pthread_mutex_init(&data.lock, NULL))
		return (-1);
	if (pthread_cond_init(&data.cond, NULL))
		return 
	
	
	
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

static int	boucle_codeur(t_data *data, t_simu *var_simu,
	long long start_time, int i)
{
	while (++i < data->number_of_coders)
	{
		var_simu->codeur[i].id = i + 1;
		var_simu->codeur[i].right_dongle = &var_simu->mutex[i];
		var_simu->codeur[i].left_dongle = &var_simu->mutex[
			(i + 1) % data->number_of_coders];
		var_simu->codeur[i].start_time = start_time;
		var_simu->codeur[i].print = &var_simu->print;
		var_simu->codeur[i].data = data;
		var_simu->codeur[i].stop = &var_simu->stop_var;
		var_simu->codeur[i].finish = &var_simu->finish;
		var_simu->codeur[i].last_compile = start_time;
		var_simu->codeur[i].all_codeurs = var_simu->codeur;
		var_simu->codeur[i].left_locked = 0;
		var_simu->codeur[i].right_locked = 0;
		if (pthread_mutex_init(&var_simu->codeur[i].codeur_lock, NULL))
			return (destroy_free_all_mut(var_simu, i, -1));
	}
	return (create_thread(data, var_simu));
}

int	init_codeur(t_data *data, t_simu *var_simu)
{
	long long	start_time;
	int			i;

	start_time = get_timestamp_ms();
	i = -1;
	return (boucle_codeur(data, var_simu, start_time, i));
}
