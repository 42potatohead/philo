/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:21:02 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/07/30 01:34:44 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_var(t_global *global)
{
	global->must_eat = -1;
	global->nb_philo = 0;
	global->ttd = 0;
	global->tte = 0;
	global->tts = 0;
	global->sim_state = RUNNING;
}

int	arr_isdigit(char **arr)
{
	int	i;
	int	j;

	i = 1;
	while (arr[i])
	{
		j = 0;
		if (arr[i][j] == '+')
			j++;
		while (arr[i][j])
		{
			if (!(arr[i][j] >= '0' && arr[i][j] <= '9'))
				return (EXIT_FAILURE);
			j++;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int	ft_atoi(const char *str)
{
	int		i;
	int		neg;
	long	nb;

	i = 0;
	neg = 1;
	nb = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg = -1;
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * neg);
}

void	safe_print(const char *str, t_philo *philos, long ms)
{
	pthread_mutex_lock(&philos[0].global->print_mutex);
	printf("%ld %d %s\n", ms, philos->id, str);
	pthread_mutex_unlock(&philos[0].global->print_mutex);
}
