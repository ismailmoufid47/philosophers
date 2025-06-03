/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:04:48 by isel-mou          #+#    #+#             */
/*   Updated: 2025/06/03 17:16:25 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ----------------------------------------------------------------------------
// Philosopher eat/sleep/think routine
void	philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	log_action(philo, "has taken a fork");
	if (philo->left_fork == philo->right_fork)
	{
		pthread_mutex_unlock(philo->left_fork);
		ft_usleep(philo, philo->data->time_to.die);
		report_death(philo);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	log_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->mutexes.meal_lock);
	philo->history.last_meal = get_current_time();
	log_action(philo, "is eating");
	philo->history.meals_eaten++;
	pthread_mutex_unlock(&philo->data->mutexes.meal_lock);
	ft_usleep(philo, philo->data->time_to.eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	log_action(philo, "is sleeping");
	ft_usleep(philo, philo->data->time_to.sleep);
	log_action(philo, "is thinking");
}

// ----------------------------------------------------------------------------
// Check if simulation has ended
int	simulation_ended(t_data *data)
{
	pthread_mutex_lock(&data->mutexes.is_dead_lock);
	if (data->is_dead)
	{
		pthread_mutex_unlock(&data->mutexes.is_dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->mutexes.is_dead_lock);
	return (0);
}

// ----------------------------------------------------------------------------
// Monitor thread
void	*monitor(void *ptr)
{
	t_philo	*philos;
	t_data	*data;

	philos = (t_philo *)ptr;
	data = philos[0].data;
	while (!simulation_ended(data) && data->philo_count > 1)
	{
		if (are_you_dead(philos))
		{
			report_death(philos);
			return (NULL);
		}
		if (are_you_full(philos))
			return (NULL);
	}
	return (NULL);
}

// ----------------------------------------------------------------------------
// Philosopher thread
void	*philosopher(void *ptr)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)ptr;
	data = philo->data;
	pthread_mutex_lock(&data->mutexes.meal_lock);
	philo->history.last_meal = data->time_to.start;
	pthread_mutex_unlock(&data->mutexes.meal_lock);
	if (philo->id % 2 == 0)
		usleep(200);
	while (!simulation_ended(data))
		philo_routine(philo);
	return (NULL);
}

// ----------------------------------------------------------------------------
// Launch all threads
int	launch_simulation(t_data *data)
{
	pthread_t	monitor_id;
	int			i;

	data->time_to.start = get_current_time();
	i = -1;
	while (++i < data->philo_count)
	{
		if (pthread_create(&data->philos[i].thread_id, NULL,
				philosopher, &data->philos[i]))
			return (destroy_all(data, "Thread Creation ERROR\n"), 0);
	}
	if (pthread_create(&monitor_id, NULL, monitor, data->philos))
		return (destroy_all(data, "Thread Creation ERROR\n"), 0);
	pthread_join(monitor_id, NULL);
	i = -1;
	while (++i < data->philo_count)
		pthread_join(data->philos[i].thread_id, NULL);
	return (1);
}
