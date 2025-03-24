/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:30:34 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/24 17:32:51 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_p)
	{
		pthread_mutex_destroy(&data->forks[i]);
		free(data->phils[i]);
	}
	free(data->phils);
	free(data->threads);
	free(data->forks);
	pthread_mutex_destroy(data->p_lock);
	free(data->p_lock);
	free(data);
}

time_t	time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	log_action(t_philo *phil, const char *action)
{
	pthread_mutex_lock(phil->data->p_lock);
	printf("%-10ld %d %s\n",
		time_ms() - phil->data->s_time, phil->id, action);
	pthread_mutex_unlock(phil->data->p_lock);
}

void	*malloc_w(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		printf("Malloc failed\n");
		exit(1);
	}
	return (ptr);
}

unsigned long long	atoull(t_data *data, const char *str)
{
	unsigned long long	res;
	int					i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	res = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9' || res > res * 10 + (str[i] - '0'))
		{
			free(data);
			printf("Error: Invalid argument\n");
			exit(1);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res);
}
