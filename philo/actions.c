/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:20:45 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/24 16:29:47 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	think(t_philo *phil)
{
	if (phil->data->is_dead)
		return ;
	log_action(phil, "is thinking");
}

void	pick_up_forks(t_philo *phil)
{
	if (phil->data->is_dead)
		return ;
	pthread_mutex_lock(&phil->data->forks[(phil->id + 1) % phil->data->n_p]);
	log_action(phil, "has taken a fork");
	if ((phil->id + 1) % phil->data->n_p == phil->id)
	{
		phil->data->is_dead = 1;
		return ;
	}
	pthread_mutex_lock(&phil->data->forks[phil->id]);
	log_action(phil, "has taken a fork");
}

void	eat(t_philo *phil)
{
	if (phil->data->is_dead)
		return ;
	log_action(phil, "is eating");
	usleep(phil->data->time_to_eat * 1000);
	phil->last_meal = time_ms();
}

void	put_down_forks(t_philo *phil)
{
	if (phil->data->is_dead)
		return ;
	pthread_mutex_unlock(&phil->data->forks[phil->id]);
	pthread_mutex_unlock(&phil->data->forks[(phil->id + 1) % phil->data->n_p]);
}

void	sleep_philo(t_philo *phil)
{
	if (phil->data->is_dead)
		return ;
	log_action(phil, "is sleeping");
	usleep(phil->data->time_to_sleep * 1000);
}
