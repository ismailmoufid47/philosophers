/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:10:12 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/21 22:09:26 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;
	int		i;

	data = malloc_w(sizeof(t_data));
	*data = (t_data){malloc_w(sizeof(t_mutex)),
		malloc_w(sizeof(t_mutex) * PHILOSOPHERS),
		malloc_w(sizeof(t_philo *) * PHILOSOPHERS),
		malloc_w(sizeof(t_thread) * PHILOSOPHERS), 0, time_ms()};
	pthread_mutex_init(data->p_lock, NULL);
	i = -1;
	while (++i < PHILOSOPHERS)
	{
		data->phils[i] = malloc_w(sizeof(t_philo));
		*data->phils[i] = (t_philo){i, data,
			time_ms(), malloc_w(sizeof(t_mutex))};
		pthread_mutex_init(data->phils[i]->meal_lock, NULL);
	}
	return (data);
}

void	*philsopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->data->is_dead)
	{
		think(philo);
		pick_up_forks(philo);
		eat(philo);
		put_down_forks(philo);
	}
	return (NULL);
}

int	monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < PHILOSOPHERS)
		{
			pthread_mutex_lock(data->phils[i]->meal_lock);
			if (time_ms() - data->phils[i]->last_meal > TIME_TO_DIE)
			{
				log_action(data->phils[i], "died");
				free_data(data);
				data->is_dead = 1;
				return (1);
			}
			pthread_mutex_unlock(data->phils[i]->meal_lock);
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_thread	monitor_thread;
	int			i;

	data = init_data(argc, argv);
	i = -1;
	while (++i < PHILOSOPHERS)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_create(&data->threads[i], NULL, philsopher, data->phils[i]);
	}
	i = -1;
	while (++i < PHILOSOPHERS)
		pthread_join(data->threads[i], NULL);
	pthread_join(data->threads[i], NULL);
	free_data(data);
	return (0);
}
