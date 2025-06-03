/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:51:27 by isel-mou          #+#    #+#             */
/*   Updated: 2025/06/03 17:03:53 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ----------------------------------------------------------------------------
// Log philosopher actions
void	log_action(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->data->mutexes.log_lock);
	pthread_mutex_lock(&philo->data->mutexes.is_dead_lock);
	if (philo->data->is_dead)
	{
		pthread_mutex_unlock(&philo->data->mutexes.is_dead_lock);
		pthread_mutex_unlock(&philo->data->mutexes.log_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->data->mutexes.is_dead_lock);
	printf("%ld %d %s\n",
		get_current_time() - philo->data->time_to.start,
		philo->id, action);
	pthread_mutex_unlock(&philo->data->mutexes.log_lock);
}

// ----------------------------------------------------------------------------
// Check if all philosophers have eaten enough
bool	are_you_full(t_philo *philos)
{
	int	finished;
	int	total;
	int	i;

	finished = 0;
	total = philos[0].data->philo_count;
	if (philos[0].data->must_eat == -1)
		return (false);
	i = -1;
	while (++i < total)
	{
		pthread_mutex_lock(&philos->data->mutexes.meal_lock);
		if (philos[i].history.meals_eaten >= philos[i].data->must_eat)
			++finished;
		pthread_mutex_unlock(&philos->data->mutexes.meal_lock);
	}
	if (finished == total)
	{
		pthread_mutex_lock(&philos->data->mutexes.is_dead_lock);
		philos[0].data->is_dead = 1;
		pthread_mutex_unlock(&philos->data->mutexes.is_dead_lock);
	}
	return (finished == total);
}

// ----------------------------------------------------------------------------
// Check if a philosopher has died
bool	check_philosopher_death(t_philo *philo)
{
	bool	is_dead;

	is_dead = false;
	pthread_mutex_lock(&philo->data->mutexes.meal_lock);
	if (get_current_time() - philo->history.last_meal
		>= philo->data->time_to.die)
		is_dead = true;
	pthread_mutex_unlock(&philo->data->mutexes.meal_lock);
	return (is_dead);
}

// ----------------------------------------------------------------------------
// Report philosopher death and set death flag
void	report_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutexes.is_dead_lock);
	printf("%ld %d died\n", get_current_time()
		- philo->data->time_to.start, philo->id);
	philo->data->is_dead = 1;
	pthread_mutex_unlock(&philo->data->mutexes.is_dead_lock);
}

// ----------------------------------------------------------------------------
// Check all philosophers for death
bool	are_you_dead(t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < philos[0].data->philo_count)
	{
		if (check_philosopher_death(&philos[i]))
			return (true);
	}
	return (false);
}
