/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:05:15 by isel-mou          #+#    #+#             */
/*   Updated: 2025/05/07 16:46:39 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	sem_close(data->sems.forks);
	sem_close(data->sems.meal_lock);
	sem_close(data->sems.log_lock);
	sem_close(data->sems.is_dead);
	sem_close(data->sems.must_eat);
	sem_unlink("/forks");
	sem_unlink("/meal_lock");
	sem_unlink("/log_lock");
	sem_unlink("/is_dead");
	sem_unlink("/must_eat");
	error_message(str, signal);
}

// ----------------------------------------------------------------------------
// Log philosopher actions
void	log_action(t_philo *philo, char *action)
{
	sem_wait(philo->data->sems.log_lock);
	printf("%ld %d %s\n",
		get_current_time() - philo->data->time_to.start,
		philo->id, action);
	if (action[0] != 'd')
		sem_post(philo->data->sems.log_lock);
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
void	ft_usleep(time_t mls)
{
	time_t	start;

	start = get_current_time();
	while (get_current_time() - start < mls)
		usleep(100);
}
