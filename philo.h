/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:52:22 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 02:34:26 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum simulation_state
{
	RUNNING,
	STOPPED,
}						t_simulation_state;

typedef struct s_global
{
	unsigned int		nb_philo;
	pthread_mutex_t		*fork_mtx;
	unsigned int		ttd;
	unsigned int		tte;
	unsigned int		tts;
	_Atomic int			must_eat;
	struct timeval		t_start;
	pthread_t			*tid;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		sim_state_mtx;
	t_simulation_state	sim_state;
}						t_global;

typedef struct s_philo
{
	_Atomic int				id;
	_Atomic unsigned int	philo_id;
	unsigned int			forks[2];
	_Atomic unsigned int	nb_meals;
	struct timeval			t_last_meal;
	pthread_mutex_t			last_meal_mtx;
	t_global				*global;
}						t_philo;

// philo

void					init_var(t_global *global);
int						check_args(t_global *global, int ac, char **argv);
int						init_mtx(t_philo *philo);
int					init_philos(t_philo **philos, t_global *global);

// sim
void					init_threads(t_philo *philos, unsigned int nb_threads);
void					*sim_threads(void *arg);

// utilities
int						ft_atoi(const char *str);
void					ft_putstr_fd(char *s, int fd);
int						arr_isdigit(char **arr);
int					clean_exit(t_philo *philo, int value, char *str);
long					time_since(struct timeval *end);
void					sleep_intervals(t_philo *philo, int ms);
void					safe_print(const char *str, t_philo *philos, long ms);
void					join_threads(t_philo *philos);

#endif
