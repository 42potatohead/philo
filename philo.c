#include "philo.h"

int any_philo_dead(t_philo *philo)
{
	unsigned int i;

	i = 0;
	while (i < philo->global->nb_philo)
	{
		pthread_mutex_lock(&philo[i].global->last_meal);
		if (time_since(&philo[i].t_last_meal) > philo[i].global->ttd)
		{
			pthread_mutex_unlock(&philo[i].global->last_meal);
			return 1; // At least one philosopher is dead
		}
		pthread_mutex_unlock(&philo[i].global->last_meal);
		i++;
	}
	return 0; // No philosophers are dead
}

int enough_eating(t_philo *philo)
{
	unsigned int i;

	i = 0;
	while (i < philo->global->nb_philo)
	{
		if (philo[i].nb_meals < philo->global->must_eat)
			return 0; // Not all philosophers have eaten enough
		i++;
	}
	return 1; // All philosophers have eaten enough
}

int is_dead(t_philo *philo)
{
	if (any_philo_dead(philo))
	{
		pthread_mutex_lock(&philo->global->sim_state_mtx);
		philo->global->sim_state = STOPPED; // Set simulation state to stopped
		pthread_mutex_unlock(&philo->global->sim_state_mtx);
		pthread_mutex_lock(&philo->global->print_mutex);
		printf("%ld %d is dead\n", time_since(&philo->global->t_start), philo->id);
		pthread_mutex_unlock(&philo->global->print_mutex);
		return 1; // Philosopher is dead
	}
	if (enough_eating(philo))
	{
		return 1;
	}
	return 0; // Philosopher is alive
}

void simulate(t_philo *philos)
{
	init_threads(philos, philos[0].global->nb_philo);

	while(1)
	{
		pthread_mutex_lock(&philos[0].global->sim_state_mtx);
		if (philos[0].global->sim_state == STOPPED)
		{
			pthread_mutex_unlock(&philos[0].global->sim_state_mtx);
			break; // Exit the loop if simulation is stopped
		}
		pthread_mutex_unlock(&philos[0].global->sim_state_mtx);
		if (is_dead(philos))
		{
			pthread_mutex_lock(&philos[0].global->sim_state_mtx);
			philos[0].global->sim_state = STOPPED;
			pthread_mutex_unlock(&philos[0].global->sim_state_mtx);
			break; // Exit the loop if a philosopher is dead
		}
		usleep(500); // Prevent busy waiting
	}
	// clean_exit(philos, EXIT_SUCCESS, NULL);
}

int main(int ac, char **argv)
{
	t_philo *philos;
	t_global *global;

	// Allocate and initialize global data first
	global = malloc(sizeof(t_global));
	if (!global)
		return (EXIT_FAILURE);
	init_var(global);
	if (check_args(global, ac, argv))
		return (EXIT_FAILURE);

	// Now allocate array of philosophers
	init_philos(&philos, global);
	init_mtx(philos);
	simulate(philos);
	// pthread_join(, NULL);
	clean_exit(philos, EXIT_SUCCESS, NULL);
	printf(" Philos: %d\n Time to Die: %d\n Time to Eat: %d\n Time to Sleep: %d\n", global->nb_philo, global->ttd, global->tte, global->tts);
	// if (ac == 6)
	// printf(" Meals to Eat %d\n", global.must_eat);
}

// gettimeofday(&global.t_start, NULL);
// usleep(50000 * 1000);
// gettimeofday(&global.t_end, NULL);
// printf("slept for %ld ms\n",((((global.t_end.tv_sec * 1000000) + global.t_end.tv_usec) - ((global.t_start.tv_sec * 1000000) + global.t_start.tv_usec)) ) / 1000 );
