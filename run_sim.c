#include "philo.h"

void philo_think(t_philo *philo)
{
	printf("%ld %d is thinking\n", time_since(&philo->global->t_start), philo->id);
}

void philo_sleep(t_philo *philo)
{
	printf("%ld %d is sleeping\n", time_since(&philo->global->t_start), philo->id);
	usleep(philo->global->tts * 1000);
}

void philo_eat(t_philo *philo, int fork1, int fork2)
{
	pthread_mutex_lock(&philo->global->fork_mtx[fork1]);
	pthread_mutex_lock(&philo->global->fork_mtx[fork2]);
	philo->nb_meals++;
	gettimeofday(&philo->t_last_meal, NULL);
	printf("%ld %d is eating\n", time_since(&philo->global->t_start), philo->id);
	usleep(philo->global->tte * 1000); // Simulate eating time
	pthread_mutex_unlock(&philo->global->fork_mtx[fork2]);
	pthread_mutex_unlock(&philo->global->fork_mtx[fork1]);
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
		pthread_mutex_lock(&philo->global->sim_state_mtx);
		if (philo->global->sim_state == STOPPED)
		{
			pthread_mutex_unlock(&philo->global->sim_state_mtx);
			break; // Exit the loop if simulation is stopped
		}
		pthread_mutex_unlock(&philo->global->sim_state_mtx);
		philo_eat(philo, fork1, fork2);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
