/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mtx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:20:36 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 02:36:12 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_meal_mtxs(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < (int)philos[0].global->nb_philo)
	{
		pthread_mutex_init(&philos[i].last_meal_mtx, NULL);
		i++;
	}
}

void destroy_forks(t_philo *philos, int i)
{
	if (philos[0].global->fork_mtx)
	{
		i = 0;
		while (i > 0)
		{
			pthread_mutex_destroy(&philos[0].global->fork_mtx[i]);
			i++;
		}
		free(philos[0].global->fork_mtx);
		if (philos[0].global)
			free(philos[0].global);
		free(philos);
	}
}

/*
   Initialize mutexes for philosophers.
   Allocates memory for fork mutexes and initializes them.
*/
int	init_mtx(t_philo *philos)
{
	int	i;

	i = 0;
	philos[0].global->fork_mtx = malloc(sizeof(pthread_mutex_t)
		* philos[0].global->nb_philo);
	if (!philos[0].global->fork_mtx)
		return (EXIT_FAILURE);
	while (i < (int)philos[0].global->nb_philo)
	{
		if (pthread_mutex_init(&philos[0].global->fork_mtx[i], NULL) == 0)
			i++;
		else
		{
			destroy_forks(philos, i);
			return (EXIT_FAILURE);
		}
	}
	pthread_mutex_init(&philos[0].global->sim_state_mtx, NULL);
	pthread_mutex_init(&philos[0].global->print_mutex, NULL);
	init_meal_mtxs(philos);
	return (EXIT_SUCCESS);
}
