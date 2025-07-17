#include "philo.h"

void init_threads(t_philo *philos, unsigned int nb_threads)
{
	pthread_t *tid;
	unsigned int i;

	tid = malloc(sizeof(pthread_t) * nb_threads);
	if (!tid)
		clean_exit(philos, 2, "Error mallocing threads\n");
	i = 0;
	// printf("threads = %d\n", nb_threads);
	gettimeofday(&philos[0].global->t_start, NULL);
	while (i < nb_threads)
	{
		if (pthread_create(&tid[i], NULL, sim_threads, &philos[i]) != 0)
			clean_exit(philos, 2, "error creating threads");
		i++;
	}
}
