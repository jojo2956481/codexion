/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 11:36:16 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/07/27 15:40:12 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_last_compile(t_codeur *c)
{
	long long	t;

	pthread_mutex_lock(&c->codeur_lock);
	t = c->last_compile;
	pthread_mutex_unlock(&c->codeur_lock);
	return (t);
}

void	update_last_compile(t_codeur *c)
{
	pthread_mutex_lock(&c->codeur_lock);
	c->last_compile = get_timestamp_ms();
	pthread_mutex_unlock(&c->codeur_lock);
}
