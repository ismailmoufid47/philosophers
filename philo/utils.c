/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:30:34 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/26 21:41:59 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_data(t_data *data)
{
	long	i;

	i = 0;
	while (i < data->n_p)
	{
		pthread_mutex_destroy(&data->forks[i]);
		free(data->phils[i]);
		i++;
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
	if (phil->data->done)
	{
		pthread_mutex_unlock(phil->data->p_lock);
		return ;
	}
	pthread_mutex_lock(phil->data->p_lock);
	printf("%-10ld %d %s\n",
		time_ms() - phil->data->s_time, phil->id + 1, action);
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

long	ft_atl(const char *str)
{
	long	res;
	int		i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	res = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9' || res > res * 10 + (str[i] - '0'))
			return (0);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res);
}
