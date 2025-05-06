/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:51:27 by isel-mou          #+#    #+#             */
/*   Updated: 2025/05/06 21:33:04 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// ----------------------------------------------------------------------------
// Check if a philosopher has died
void	check_philosopher_death(t_philo *philo)
{
	t_data	*data;
	int		i;

	data = philo->data;
	if (get_current_time() - philo->history.last_meal >= data->time_to.die)
	{
		log_action(philo, "died");
		sem_post(data->sems.meal_lock);
		i = -1;
		while (++i < data->philo_count)
			sem_post(data->sems.must_eat);
		sem_post(data->sems.is_dead);
		exit(EXIT_FAILURE);
	}
}
