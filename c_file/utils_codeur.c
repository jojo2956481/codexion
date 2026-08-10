/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_codeur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:03:06 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/10 14:28:26 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	manage_codeur(t_codeur *codeur, int i)
{
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
