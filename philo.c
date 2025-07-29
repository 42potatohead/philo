/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:20:19 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 02:37:36 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
   Check if any philosopher is dead.
   Returns 1 if at least one philosopher is dead, 0 otherwise.
*/
int	any_philo_dead(t_philo *philo)
{
	unsigned int	i;
	long			death_time;

	i = 0;
	while (i < philo->global->nb_philo)
	{
		pthread_mutex_lock(&philo[i].last_meal_mtx);
		death_time = time_since(&philo[i].t_last_meal);
		if (death_time > philo[i].global->ttd)
		{
			pthread_mutex_unlock(&philo[i].last_meal_mtx);
			pthread_mutex_lock(&philo[i].global->print_mutex);
			printf("%ld %d died\n", death_time, philo[i].id);
			pthread_mutex_unlock(&philo[i].global->print_mutex);
			return (1);
		}
		pthread_mutex_unlock(&philo[i].last_meal_mtx);
		i++;
	}
	return (0);
}

/*
   Check if all philosophers have eaten enough meals.
   Returns 1 if all philosophers have eaten enough, 0 otherwise.
 */
int	enough_eating(t_philo *philo)
{
	unsigned int	i;

	i = 0;
	while (i < philo->global->nb_philo)
	{
		pthread_mutex_lock(&philo[i].last_meal_mtx);
		if (philo[i].nb_meals < philo->global->must_eat)
		{
			pthread_mutex_unlock(&philo[i].last_meal_mtx);
			return (0);
		}
		pthread_mutex_unlock(&philo[i].last_meal_mtx);
		i++;
	}
	return (1);
}

int	is_dead(t_philo *philo)
{
	if (any_philo_dead(philo))
	{
		pthread_mutex_lock(&philo->global->sim_state_mtx);
		philo->global->sim_state = STOPPED;
		pthread_mutex_unlock(&philo->global->sim_state_mtx);
		return (1);
	}
	if (enough_eating(philo))
	{
		pthread_mutex_lock(&philo->global->sim_state_mtx);
		philo->global->sim_state = STOPPED;
		pthread_mutex_unlock(&philo->global->sim_state_mtx);
		return (1);
	}
	return (0);
}

/*
   monitor function to check the state of philosophers.
   usleep for a short duration to prevent busy waiting. (busy waiting is
   when a thread continuously checks a condition without yielding the CPU)
*/
void	simulate(t_philo *philos)
{
	init_threads(philos, philos[0].global->nb_philo); // protect 
	while (1)
	{
		pthread_mutex_lock(&philos[0].global->sim_state_mtx);
		if (philos[0].global->sim_state == STOPPED)
		{
			pthread_mutex_unlock(&philos[0].global->sim_state_mtx);
			break ;
		}
		pthread_mutex_unlock(&philos[0].global->sim_state_mtx);
		if (is_dead(philos))
		{
			break ;
		}
		usleep(10);
	}
}

int	main(int ac, char **argv)
{
	t_philo		*philos;
	t_global	*global;

	if (ac < 5 || ac > 6)
	{
		ft_putstr_fd("wrong number of arguments\n", 2);
		return (EXIT_FAILURE);
	}
	global = malloc(sizeof(t_global));
	if (!global)
		return (EXIT_FAILURE);
	init_var(global);
	if (check_args(global, ac, argv))
		return (EXIT_FAILURE);
	if (init_philos(&philos, global) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if(init_mtx(philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	simulate(philos);
	join_threads(philos);
	return (clean_exit(philos, EXIT_SUCCESS, NULL));
}
