/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:20:45 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/21 22:01:54 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	think(t_philo *philo)
{
	log_action(philo, "is thinking");
	usleep(500 * 1000);
}

void	pick_up_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->id]);
	log_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[(philo->id + 1) % PHILOSOPHERS]);
	log_action(philo, "has taken a fork");
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = time_ms();
	log_action(philo, "is eating");
	pthread_mutex_unlock(philo->meal_lock);
	usleep(TIME_TO_EAT * 1000);
}

void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[philo->id]);
	pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) % PHILOSOPHERS]);
}

void	sleep_philo(t_philo *philo)
{
	log_action(philo, "is sleeping");
	usleep(TIME_TO_SLEEP * 1000);
}
