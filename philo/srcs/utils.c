/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:05:15 by isel-mou          #+#    #+#             */
/*   Updated: 2025/05/06 21:44:06 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ----------------------------------------------------------------------------
// Print error message and exit
void	error_message(char *text, int signal)
{
	int		length;

	if (text)
	{
		length = 0;
		while (text[length])
			length++;
		write(2, text, length);
	}
	exit(signal);
}

// ----------------------------------------------------------------------------
// Free all allocated resources
void	destroy_all(t_data *data, char *str, int signal)
{
	while (data->philo_count--)
		pthread_mutex_destroy(&data->mutexes.forks[data->philo_count]);
	pthread_mutex_destroy(&data->mutexes.log_lock);
	pthread_mutex_destroy(&data->mutexes.meal_lock);
	pthread_mutex_destroy(&data->mutexes.is_dead_lock);
	error_message(str, signal);
}

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
// Get current time in milliseconds
time_t	get_current_time(void)
{
	t_timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		error_message("gettimeofday ERROR\n", 1);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

// ----------------------------------------------------------------------------
// More precise sleep function
void	ft_usleep(t_philo *philo, time_t mls)
{
	time_t	start;

	pthread_mutex_lock(&philo->data->mutexes.is_dead_lock);
	if (philo->data->is_dead)
	{
		pthread_mutex_unlock(&philo->data->mutexes.is_dead_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->data->mutexes.is_dead_lock);
	start = get_current_time();
	while (get_current_time() - start < mls)
		usleep(100);
}
