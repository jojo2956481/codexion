/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:02:54 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 13:43:42 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	code_error_parsing(int error)
{
	if (error == -1)
		printf("Error: the 7th first datas must be a number\n");
	if (error == -2)
		printf("Error: the number must be a 32-bit integer\n");
	if (error == -3)
		printf("Error: the number of coders must be at least 1\n");
	if (error == -4)
		printf("Error: scheduler must be 'fifo', 'lifo' or 'edf'\n");
	if (error == -5)
		printf("Error: the number of compiles must be at least 1\n");
}

void	code_error_simulation(int error)
{
	if (error == -1)
		printf("Error: init_mutex()\n");
	if (error == -2)
		printf("Error: malloc()\n");
	if (error == -3)
		printf("Error: pthread_create()\n");
}

int	ft_isdigit(int c)
{
	if (c < 48 || c > 57)
		return (0);
	return (1);
}

int	init_data(char **datav, t_data *data)
{
	if (atoi(datav[1]) < 1)
		return (-3);
	if (atoi(datav[6]) < 1)
		return (-5);
	data->number_of_coders = atoi(datav[1]);
	data->time_to_burnout = atoi(datav[2]);
	data->time_to_compile = atoi(datav[3]);
	data->time_to_debug = atoi(datav[4]);
	data->time_to_refactor = atoi(datav[5]);
	data->number_of_compiles_required = atoi(datav[6]);
	data->dongle_cooldown = atoi(datav[7]);
	if (strcmp(datav[8], "fifo") && strcmp(datav[8], "lifo")
		&& strcmp(datav[8], "edf"))
		return (-4);
	data->scheduler = datav[8];
	data->queue.coder_ids = NULL;
	return (0);
}

long long	ft_atoi(char *nptr)
{
	long long	resultat;
	int			i;

	i = 0;
	resultat = 0;
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		if (i > 10)
			return (-1);
		resultat = resultat * 10 + (nptr[i] - '0');
		i++;
	}
	return (resultat);
}
