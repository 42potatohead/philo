/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   short_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:20:54 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 01:20:55 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleep_intervals(t_philo *philo, int ms)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (time_since(&start) < ms)
	{
		usleep(100);
		pthread_mutex_lock(&philo->global->sim_state_mtx);
		if (philo->global->sim_state != RUNNING)
		{
			pthread_mutex_unlock(&philo->global->sim_state_mtx);
			break ;
		}
		pthread_mutex_unlock(&philo->global->sim_state_mtx);
	}
}
