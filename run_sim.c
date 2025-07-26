#include "philo.h"

int check_state(t_philo *philo)
{
		pthread_mutex_lock(&philo->global->sim_state_mtx);
		if (philo->global->sim_state == STOPPED)
		{
			pthread_mutex_unlock(&philo->global->sim_state_mtx);
			return 1; // Exit the loop if simulation is stopped
		}
		pthread_mutex_unlock(&philo->global->sim_state_mtx);
		return 0;
}

void philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->global->print_mutex);
	printf("%ld %d is thinking\n", time_since(&philo->global->t_start), philo->id);
	pthread_mutex_unlock(&philo->global->print_mutex);

}

void philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->global->print_mutex);
	printf("%ld %d is sleeping\n", time_since(&philo->global->t_start), philo->id);
	pthread_mutex_unlock(&philo->global->print_mutex);

	usleep(philo->global->tts * 1000);
}

void philo_eat(t_philo *philo, int fork1, int fork2)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2 == 0)
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
	pthread_mutex_lock(second_fork);
	if (check_state(philo) == 1)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	philo->nb_meals++;
	pthread_mutex_lock(&philo->global->last_meal);
	if (check_state(philo) == 1)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(&philo->global->last_meal);
		return ;
	}
	gettimeofday(&philo->t_last_meal, NULL);
	pthread_mutex_unlock(&philo->global->last_meal);
	pthread_mutex_lock(&philo->global->print_mutex);
	printf("%ld %d is eating\n", time_since(&philo->global->t_start), philo->id);
	pthread_mutex_unlock(&philo->global->print_mutex);
	usleep(philo->global->tte * 1000); // Simulate eating time
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

void *sim_threads(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	int fork1;
	int fork2;

	// Initialize forks for the philosopher
	fork1 = philo->forks[0];
	fork2 = philo->forks[1];
	philo->t_last_meal = philo->global->t_start;
	// printf("hello this is philo [%d]\n", philo->id);
	// printf("Forks: %d and %d\n", fork1, fork2);
	while (1)
	{
		if (check_state(philo) == 1)
			break;
		philo_eat(philo, fork1, fork2);
		philo_sleep(philo);
		if (check_state(philo) == 1)
			break;
		philo_think(philo);
	}
	return (NULL);
}
