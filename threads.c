/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:20:57 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 02:19:20 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_threads(t_philo *philos, unsigned int nb_threads)
{
	unsigned int	i;

	philos[0].global->tid = malloc(sizeof(pthread_t) * nb_threads);
	if (!philos[0].global->tid)
		clean_exit(philos, 2, "Error mallocing threads\n");
	i = 0;
	gettimeofday(&philos[0].global->t_start, NULL);
	while (i < nb_threads)
	{
		if (pthread_create(&philos[0].global->tid[i]
				, NULL, sim_threads, &philos[i]) != 0)
			clean_exit(philos, 2, "error creating threads");
		i++;
	}
}

void	join_threads(t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos[0].global->nb_philo)
	{
		pthread_join(philos[0].global->tid[i], NULL);
		i++;
	}
}
