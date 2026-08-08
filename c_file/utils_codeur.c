/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_codeur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:15:25 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 by lebeyssa                ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	manage_codeur(t_codeur *codeur, int i, t_dongle *first, t_dongle *second)
{
	(void)first;
	(void)second;
	while (i < codeur->data->number_of_compiles_required)
	{
		if (take_dongles(codeur))
			return (-1);
		if (check_stop_release(codeur, 1))
			return (-1);
		if (check_stop_display(codeur, " is compiling\n", 1))
			return (-1);
		if (check_sleep_release(codeur))
			return (-1);
		if (check_stop_release(codeur, 0))
			return (-1);
		if (check_stop_display(codeur, " is debugging\n", 0))
			return (-1);
		if (interruptible_sleep(codeur, codeur->data->time_to_debug))
			return (-1);
		if (check_stop_release(codeur, 0))
			return (-1);
		if (check_stop_display(codeur, " is refactoring\n", 0))
			return (-1);
		if (interruptible_sleep(codeur, codeur->data->time_to_refactor))
			return (-1);
		i++;
	}
	return (0);
}
