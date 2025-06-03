/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:05:15 by isel-mou          #+#    #+#             */
/*   Updated: 2025/06/03 17:04:43 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ----------------------------------------------------------------------------
// Print error message and exit
void	error_message(char *text)
{
	int		length;

	if (text)
	{
		length = 0;
		while (text[length])
			length++;
		write(2, text, length);
	}
}

// ----------------------------------------------------------------------------
// Free all allocated resources
void	destroy_all(t_data *data, char *str)
{
	while (data->philo_count--)
		pthread_mutex_destroy(&data->mutexes.forks[data->philo_count]);
	pthread_mutex_destroy(&data->mutexes.log_lock);
	pthread_mutex_destroy(&data->mutexes.meal_lock);
	pthread_mutex_destroy(&data->mutexes.is_dead_lock);
	error_message(str);
}

// ----------------------------------------------------------------------------
// Get current time in milliseconds
time_t	get_current_time(void)
{
	t_timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (error_message("gettimeofday ERROR\n"), -1);
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

// ----------------------------------------------------------------------------
// Convert string to long integer
long	ft_atol(const char *str)
{
	long	num;
	int		i;

	if (!str || !*str)
		return (-1);
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		++i;
	if (str[i] == '+')
		++i;
	if (str[i] == '-')
		return (-1);
	num = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		if (num > (LONG_MAX / 10) || (num == (LONG_MAX / 10) && str[i] > '7'))
			return (-1);
		num = num * 10 + (str[i] - '0');
		++i;
	}
	if (!str[i] && !num)
		return (-1);
	return (num);
}
