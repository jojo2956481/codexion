/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 16:42:04 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 13:46:48 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_codeur(t_data *data, t_simu *var_simu)
{
	long long	start_time;
	int			i;

	start_time = get_timestamp_ms();
	i = -1;
	return (boucle_codeur(data, var_simu, start_time, i));
}

int	create_thread(t_data *data, t_simu *var_simu)
{
	int	i;

	i = 0;
	if (pthread_create(&var_simu->t2, NULL,
			pthread_moniteur, &var_simu->monitor))
		return (destroy_free_all(var_simu, i, 0));
	if (pthread_create(&var_simu->ticker, NULL, queue_ticker, data))
		return (destroy_free_all(var_simu, i, 1));
	while (i < data->number_of_coders)
	{
		if (pthread_create(&var_simu->threads[i],
				NULL, pthread_codeur, &var_simu->codeur[i]))
			return (destroy_free_all(var_simu, i, 1));
		i++;
	}
	return (0);
}
