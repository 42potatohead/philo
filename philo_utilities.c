/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:20:41 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 02:34:14 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
   Initialize philosophers.
   Allocates memory for philosophers and initializes their attributes.
*/
int	init_philos(t_philo **philos, t_global *global)
{
	unsigned int	i;

	*philos = malloc(sizeof(t_philo) * global->nb_philo);
	if (!*philos)
		return(clean_exit(NULL, EXIT_FAILURE, "malloc error"));
	gettimeofday(&global->t_start, NULL);
	i = 0;
	while (i < global->nb_philo)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].philo_id = i;
		(*philos)[i].forks[0] = i;
		(*philos)[i].forks[1] = (i + 1) % global->nb_philo;
		(*philos)[i].nb_meals = 0;
		(*philos)[i].global = global;
		(*philos)[i].t_last_meal = global->t_start;
		i++;
	}
	return (EXIT_SUCCESS);
}

/*
   Check command line arguments.
   Returns EXIT_SUCCESS if valid, EXIT_FAILURE otherwise.
   initializes global variables in the t_global struct.
*/
int	check_args(t_global *global, int ac, char **argv)
{
	if (ac == 6)
		global->must_eat = ft_atoi(argv[5]);
	if (arr_isdigit(argv) == EXIT_FAILURE)
	{
		ft_putstr_fd("Arguments must be positive integers\n", 2);
		free(global);
		return (EXIT_FAILURE);
	}
	global->nb_philo = ft_atoi(argv[1]);
	global->ttd = ft_atoi(argv[2]);
	global->tte = ft_atoi(argv[3]);
	global->tts = ft_atoi(argv[4]);
	return (EXIT_SUCCESS);
}

/*
   Calculate the time since the last meal.
   Returns the time in milliseconds.
*/
long	time_since(struct timeval *end)
{
	struct timeval	current_time;

	if (!end)
		return (0);
	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec - end->tv_sec) * 1000L
		+ (current_time.tv_usec - end->tv_usec) / 1000L);
}
