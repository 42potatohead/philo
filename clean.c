/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:55:41 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 02:38:55 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mtx(t_philo *philos)
{
	unsigned int	i;

	if (philos[0].global && philos[0].global->fork_mtx)
	{
		i = 0;
		while (i < philos[0].global->nb_philo)
		{
			pthread_mutex_destroy(&philos[0].global->fork_mtx[i]);
			pthread_mutex_destroy(&philos[i].last_meal_mtx);
			i++;
		}
		free(philos[0].global->fork_mtx);
	}
	pthread_mutex_destroy(&philos[0].global->sim_state_mtx);
	pthread_mutex_destroy(&philos[0].global->print_mutex);
}

/*
   Clean up and free allocated resources.
   and exit the program with the specified value.
*/
int	clean_exit(t_philo *philos, int value, char *str)
{
	unsigned int	i;

	if (str && *str)
		ft_putstr_fd(str, 2);
	if (philos)
	{
		destroy_mtx(philos);
		i = 0;
		if (philos[0].global->tid)
			free(philos[0].global->tid);
		if (philos[0].global)
			free(philos[0].global);
		free(philos);
	}
	return (value);
}
