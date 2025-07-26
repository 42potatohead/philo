# ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

typedef enum simulation_state
{
    RUNNING,
	STOPPED,
} e_simulation_state;

// typedef enum philo_state
// {
//     EATING,
// 	SLEEPING,
// 	THINKING,
// 	DEAD
// } e_philo_state;

typedef struct s_global
{
	unsigned int nb_philo;
	pthread_mutex_t *fork_mtx;
	unsigned int ttd;
	unsigned int tte;
	unsigned int tts;
	_Atomic int must_eat;
	struct timeval t_start;
	pthread_mutex_t print_mutex;
	pthread_mutex_t last_meal;
	pthread_mutex_t sim_state_mtx;
	e_simulation_state sim_state;
} t_global;

typedef struct s_philo
{
	int id;
	_Atomic unsigned int philo_id;
	unsigned int forks[2];
	_Atomic unsigned int nb_meals;
	struct timeval t_last_meal;
	t_global *global;
	// e_philo_state state;
} t_philo;



// philo

void init_var(t_global *global);
int check_args(t_global *global, int ac, char **argv);
int init_mtx(t_philo *philo);
void init_philos(t_philo **philos, t_global *global);

// sim
void init_threads(t_philo *philos, unsigned int nb_threads);
void *sim_threads(void *arg);

// utilities
int	ft_atoi(const char *str);
void	ft_putstr_fd(char *s, int fd);
int arr_isdigit(char **arr);
void clean_exit(t_philo *philo, int value, char *str);
long time_since(struct timeval *end);

#endif
