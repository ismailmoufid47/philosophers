/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:20:45 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/26 22:54:55 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	think(t_philo *phil)
{
	log_action(phil, "is thinking");
	return (1);
}

int	pick_up_forks(t_philo *phil)
{
	pthread_mutex_lock(phil->data->done_lock);
	if (phil->data->done)
		return (0);
	pthread_mutex_unlock(phil->data->done_lock);
	if (phil->id % 2 == 0)
		pthread_mutex_lock(&phil->data->forks[phil->id]);
	else
		pthread_mutex_lock(
			&phil->data->forks[(phil->id + 1) % phil->data->n_p]);
	if (phil->data->done)
		return (0);
	log_action(phil, "has taken a fork");
	if (phil->data->n_p == 1)
		return (0);
	if (phil->id % 2 == 0)
		pthread_mutex_lock(
			&phil->data->forks[(phil->id + 1) % phil->data->n_p]);
	else
		pthread_mutex_lock(&phil->data->forks[phil->id]);
	log_action(phil, "has taken a fork");
	return (1);
}

int	eat(t_philo *phil)
{
	time_t	start;

	log_action(phil, "is eating");
	phil->n_meals++;
	phil->last_meal = time_ms();
	start = time_ms();
	while (time_ms() - start < phil->data->time_to_eat)
	{
		pthread_mutex_lock(phil->data->done_lock);
		if (phil->data->done)
			put_down_forks(phil);
		pthread_mutex_unlock(phil->data->done_lock);
		usleep(200);
	}
	put_down_forks(phil);
	return (1);
}

void	put_down_forks(t_philo *phil)
{
	pthread_mutex_unlock(&phil->data->forks[phil->id]);
	pthread_mutex_unlock(&phil->data->forks[(phil->id + 1) % phil->data->n_p]);
}

int	sleep_philo(t_philo *phil)
{
	time_t	start;

	log_action(phil, "is sleeping");
	start = time_ms();
	while (time_ms() - start < phil->data->time_to_sleep)
		usleep(200);
	return (1);
}
