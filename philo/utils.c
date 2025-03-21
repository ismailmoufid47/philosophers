/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:30:34 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/21 22:05:25 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < PHILOSOPHERS)
		free(data->phils[i]);
	pthread_mutex_destroy(data->p_lock);
	free(data->p_lock);
	free(data);
}

t_time	time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	log_action(t_philo *philo, const char *action)
{
	pthread_mutex_lock(philo->data->p_lock);
	printf("%-10lld %d %s\n",
		time_ms() - philo->data->s_time, philo->id, action);
	pthread_mutex_unlock(philo->data->p_lock);
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
