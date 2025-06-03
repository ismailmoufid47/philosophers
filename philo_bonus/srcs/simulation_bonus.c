/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:04:48 by isel-mou          #+#    #+#             */
/*   Updated: 2025/05/06 21:30:43 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// ----------------------------------------------------------------------------
// Philosopher eat/think/sleep routine
void	philo_routine(t_philo *philo)
{
	sem_wait(philo->data->sems.forks);
	log_action(philo, "has taken a fork");
	sem_wait(philo->data->sems.forks);
	log_action(philo, "has taken a fork");
	sem_wait(philo->data->sems.meal_lock);
	philo->history.last_meal = get_current_time();
	log_action(philo, "is eating");
	philo->history.meals_eaten++;
	sem_post(philo->data->sems.meal_lock);
	ft_usleep(philo->data->time_to.eat);
	sem_post(philo->data->sems.forks);
	sem_post(philo->data->sems.forks);
	log_action(philo, "is sleeping");
	ft_usleep(philo->data->time_to.sleep);
	log_action(philo, "is thinking");
}

// ----------------------------------------------------------------------------
// Monitor one philosopher
void	*monitor(void *ptr)
{
	t_philo	*philo;
	t_data	*data;
	bool	finished_eating;

	philo = (t_philo *)ptr;
	data = philo->data;
	finished_eating = false;
	while (1)
	{
		sem_wait(data->sems.meal_lock);
		check_philosopher_death(philo);
		if (!finished_eating && philo->data->meals_to_eat != -1
			&& philo->history.meals_eaten >= data->meals_to_eat)
		{
			finished_eating = true;
			sem_post(data->sems.must_eat);
		}
		sem_post(data->sems.meal_lock);
	}
	return (NULL);
}

// ----------------------------------------------------------------------------
// Philosopher thread
void	philosopher(t_philo *philo)
{
	pthread_t	monitor_id;
	t_data		*data;

	data = philo->data;
	sem_wait(data->sems.meal_lock);
	philo->history.last_meal = data->time_to.start;
	sem_post(data->sems.meal_lock);
	if (pthread_create(&monitor_id, NULL, monitor, philo))
		destroy_all(data, "Thread Creation ERROR\n", 1);
	pthread_detach(monitor_id);
	while (1)
		philo_routine(philo);
}

void	end_simulation(t_data *data)
{
	int		i;

	if (data->meals_to_eat != -1)
	{
		i = -1;
		while (++i < data->philo_count)
			sem_wait(data->sems.must_eat);
		sem_post(data->sems.is_dead);
	}
	sem_wait(data->sems.is_dead);
	i = -1;
	while (++i < data->philo_count)
		kill(data->philos[i].pid, SIGKILL);
	i = -1;
	while (++i < data->philo_count)
		waitpid(data->philos[i].pid, NULL, 0);
}

// ----------------------------------------------------------------------------
// Launch all threads
void	launch_simulation(t_data *data)
{
	pid_t		pid;
	int			i;

	data->time_to.start = get_current_time();
	i = -1;
	while (++i < data->philo_count)
	{
		pid = fork();
		if (pid == 0)
		{
			philosopher(&data->philos[i]);
			exit(0);
		}
		else if (pid > 0)
			data->philos[i].pid = pid;
		else
			destroy_all(data, "Fork ERROR\n", 1);
	}
	end_simulation(data);
}
