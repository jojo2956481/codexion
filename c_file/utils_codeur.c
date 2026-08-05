/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_codeur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:15:25 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int take_dongles(t_codeur *c)
{
    t_data *data = c->data;
    queue_push(data, c);

    while (1)
    {
        pthread_mutex_lock(&data->queue.lock);
        
        if (check_stop(data))
        {
            pthread_mutex_unlock(&data->queue.lock);
            queue_remove(data, c->id);
            return (-1);
        }

        long long now = get_timestamp_ms();
        t_dongle *d1 = (c->left < c->right) ? c->left : c->right;
        t_dongle *d2 = (c->left < c->right) ? c->right : c->left;

        pthread_mutex_lock(&d1->mutex);
        pthread_mutex_lock(&d2->mutex);
        if (is_priority(data, c->id) &&
            !d1->is_locked && now >= d1->available_at &&
            !d2->is_locked && now >= d2->available_at)
        {
            // Acquisition réussie !
            d1->is_locked = 1;
            d2->is_locked = 1;
            
            pthread_mutex_unlock(&d2->mutex);
            pthread_mutex_unlock(&d1->mutex);
            pthread_mutex_unlock(&data->queue.lock);
            queue_remove(data, c->id);
            print_status(c, "has taken a dongle");
            print_status(c, "has taken a dongle");
            return (0);
        }

        pthread_mutex_unlock(&d2->mutex);
        pthread_mutex_unlock(&d1->mutex);

        pthread_cond_wait(&data->queue.cond, &data->queue.lock);
        pthread_mutex_unlock(&data->queue.lock);
    }
}


int	manage_codeur(t_codeur *codeur, int i, t_dongle *first, t_dongle *second)
{
	while (i < codeur->data->number_of_compiles_required)
	{
		if (take_dongle(first, second, codeur))
			return (-1);
		if (check_stop_release(first, second, codeur, 1))
			return (-1);
		if (check_stop_display(codeur, " is compiling\n", 1))
			return (-1);
		if (check_sleep_release(codeur, first, second))
			return (-1);
		if (check_stop_release(NULL, NULL, codeur, 0))
			return (-1);
		if (check_stop_display(codeur, " is debugging\n", 0))
			return (-1);
		if (interruptible_sleep(codeur, codeur->data->time_to_debug))
			return (-1);
		if (check_stop_release(NULL, NULL, codeur, 0))
			return (-1);
		if (check_stop_display(codeur, " is refactoring\n", 0))
			return (-1);
		if (interruptible_sleep(codeur, codeur->data->time_to_refactor))
			return (-1);
		i++;
	}
	return (0);
}
