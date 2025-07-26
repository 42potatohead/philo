#include "philo.h"

int init_mtx(t_philo *philos)
{
	int i = 0;
	philos[0].global->fork_mtx = malloc(sizeof(pthread_mutex_t) * philos[0].global->nb_philo);
	if (!philos[0].global->fork_mtx)
		return (EXIT_FAILURE);
	while (i < (int)philos[0].global->nb_philo)
	{
		if (pthread_mutex_init(&philos[0].global->fork_mtx[i], NULL) == 0)
			i++;
		else
		{
			// clean up previously initialized mutexes
			while (--i >= 0)
				pthread_mutex_destroy(&philos[0].global->fork_mtx[i]);
			free(philos[0].global->fork_mtx);
			return (EXIT_FAILURE);
		}
	}
	pthread_mutex_init(&philos[0].global->sim_state_mtx, NULL);
	pthread_mutex_init(&philos[0].global->last_meal, NULL);
	return (EXIT_SUCCESS);
}
void init_philos(t_philo **philos, t_global *global)
{
	unsigned int i;

	*philos = malloc(sizeof(t_philo) * global->nb_philo);
	if (!*philos)
		clean_exit(NULL, EXIT_FAILURE, "malloc error");

	// Initialize global start time
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
}
void clean_exit(t_philo *philos, int value, char *str)
{
	unsigned int i;
	if (str && *str)
		ft_putstr_fd(str, 2);
	if (philos)
	{
		if (philos[0].global && philos[0].global->fork_mtx)
		{
			i = 0;
			while (i < philos[0].global->nb_philo)
			{
				pthread_mutex_destroy(&philos[0].global->fork_mtx[i]);
				i++;
			}
			free(philos[0].global->fork_mtx);
		}
		pthread_mutex_destroy(&philos[0].global->sim_state_mtx);
		pthread_mutex_destroy(&philos[0].global->last_meal);
		if (philos[0].global)
			free(philos[0].global);
		free(philos);
	}
	exit(value);
}

int check_args(t_global *global, int ac, char **argv)
{
	if (ac < 5 || ac > 6)
	{
		ft_putstr_fd("wrong number of arguments\n", 2);
		return (EXIT_FAILURE);
	}
	else if (ac == 6)
		global->must_eat = ft_atoi(argv[5]);
	if (arr_isdigit(argv) == EXIT_FAILURE)
	{
		ft_putstr_fd("Arguments must be positive integers\n", 2);
		return (EXIT_FAILURE);
	}
	global->nb_philo = ft_atoi(argv[1]);
	global->ttd = ft_atoi(argv[2]);
	global->tte = ft_atoi(argv[3]);
	global->tts = ft_atoi(argv[4]);
	printf("ttd %d\n", global->ttd);
	printf("tte %d\n", global->tte);
	printf("tts %d\n", global->tts);
	// printf("philo nb %d\n", global->nb_philo);

	return (EXIT_SUCCESS);
}

long time_since(struct timeval *end)
{
	struct timeval current_time;
	// if (!start || !end)
	// 	return -1; // Error handling for null pointers
	// if (end->tv_sec < start->tv_sec ||
	//     (end->tv_sec == start->tv_sec && end->tv_usec < start->tv_usec))
	// 	return -1; // Error handling for end time before start time
	gettimeofday(&current_time, NULL);
	// if (current_time->tv_sec < end->tv_sec ||
	//     (current_time->tv_sec == end->tv_sec && current_time->tv_usec < end->tv_usec))
	// 	return -1; // Error handling for current time before end time
	return ((current_time.tv_sec - end->tv_sec) * 1000L
			+ (current_time.tv_usec - end->tv_usec) / 1000L);
	}
