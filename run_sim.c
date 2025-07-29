/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_sim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:20:51 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 02:46:41 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_state(t_philo *philo)
{
	pthread_mutex_lock(&philo->global->sim_state_mtx);
	if (philo->global->sim_state == STOPPED)
	{
		pthread_mutex_unlock(&philo->global->sim_state_mtx);
		return (1);
	}
	pthread_mutex_unlock(&philo->global->sim_state_mtx);
	return (0);
}

void	philo_think(t_philo *philo)
{
	safe_print("is thinking", philo, time_since(&philo->global->t_start));
	usleep(500);
}

void	philo_sleep(t_philo *philo)
{
	safe_print("is sleeping", philo, time_since(&philo->global->t_start));
	sleep_intervals(philo, philo->global->tts);
}

void	philo_eat(t_philo *philo, int fork1, int fork2)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2 != 0)
	{
		first_fork = &philo->global->fork_mtx[fork1];
		second_fork = &philo->global->fork_mtx[fork2];
	}
	else
	{
		first_fork = &philo->global->fork_mtx[fork2];
		second_fork = &philo->global->fork_mtx[fork1];
	}
	pthread_mutex_lock(first_fork);
	if (check_state(philo) == 1)
	{
		pthread_mutex_unlock(first_fork);
		return ;
	}
	safe_print("has taken a fork", philo, time_since(&philo->global->t_start));
	pthread_mutex_lock(second_fork);
	if (check_state(philo) == 1)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	safe_print("has taken a fork", philo, time_since(&philo->global->t_start));
	if (check_state(philo) == 1)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	philo->nb_meals++;
	pthread_mutex_lock(&philo->last_meal_mtx);
	gettimeofday(&philo->t_last_meal, NULL);
	pthread_mutex_unlock(&philo->last_meal_mtx);
	if (check_state(philo) == 1)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	safe_print("is eating", philo, time_since(&philo->global->t_start));
	sleep_intervals(philo, philo->global->tte);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

void	*sim_threads(void *arg)
{
	t_philo	*philo;
	int		fork1;
	int		fork2;

	philo = (t_philo *)arg;
	fork1 = philo->forks[0];
	fork2 = philo->forks[1];
	pthread_mutex_lock(&philo->last_meal_mtx);
	philo->t_last_meal = philo->global->t_start;
	pthread_mutex_unlock(&philo->last_meal_mtx);
	if (philo->global->nb_philo > 80 && philo->id % 2 == 0)
		usleep(1000);
	if (philo->global->nb_philo == 1)
	{
		safe_print("has taken a fork", philo , 0);
		return (NULL);
	}
	while (1)
	{
		if (check_state(philo) == 1)
			break ;
		philo_eat(philo, fork1, fork2);
		if (check_state(philo) == 1)
			break ;
		philo_sleep(philo);
		if (check_state(philo) == 1)
			break ;
		philo_think(philo);
	}
	return (NULL);
}
