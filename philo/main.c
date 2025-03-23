/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:10:12 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/23 22:06:54 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_data	*init_data(int argc, char **av)
{
	t_data	*data;
	int		i;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		exit(1);
	}
	data = malloc_w(sizeof(t_data));
	*data = (t_data){atol(av[1]), atot(av[2]), atot(av[3]), atot(av[4]),
		.s_time = time_ms(), .is_dead = 0, .p_lock = malloc_w(sizeof(t_mutex)),
		.forks = malloc_w(sizeof(t_mutex) * atol(av[1])),
		.phils = malloc_w(sizeof(t_philo *) * atol(av[1])),
		.threads = malloc_w(sizeof(t_thread) * atol(av[1])),};
	pthread_mutex_init(data->p_lock, NULL);
	i = -1;
	while (++i < data->n_p)
	{
		data->phils[i] = malloc_w(sizeof(t_philo));
		*data->phils[i] = (t_philo){i, data,
			time_ms(), malloc_w(sizeof(t_mutex))};
		pthread_mutex_init(data->phils[i]->meal_lock, NULL);
		pthread_mutex_init(&data->forks[i], NULL);
	}
	return (data);
}

void	*philsopher(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->id % 2 == 0)
		usleep(500);
	while (!phil->data->is_dead)
	{
		think(phil);
		pick_up_forks(phil);
		eat(phil);
		put_down_forks(phil);
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->n_p)
		{
			pthread_mutex_lock(data->p_lock);
			if (data->is_dead)
			{
				pthread_mutex_unlock(data->p_lock);
				return (NULL);
			}
			pthread_mutex_unlock(data->p_lock);

			pthread_mutex_lock(data->phils[i]->meal_lock);
			if (time_ms() - data->phils[i]->last_meal > data->time_to_die)
			{
				log_action(data->phils[i], "died");
				pthread_mutex_lock(data->p_lock);
				data->is_dead = 1;
				pthread_mutex_unlock(data->p_lock);
				pthread_mutex_unlock(data->phils[i]->meal_lock);
				return (NULL);
			}
			pthread_mutex_unlock(data->phils[i]->meal_lock);
		}
	}
}


int	main(int argc, char **argv)
{
	t_data		*data;
	t_thread	monitor_thread;
	int			i;

	data = init_data(argc, argv);
	printf("number of philosophers: %d\ntime to die: %ld\ntime to eat: %ld\ntime to sleep: %ld\n",
		data->n_p, data->time_to_die, data->time_to_eat, data->time_to_sleep);
	i = -1;
	pthread_create(&monitor_thread, NULL, monitor, data);
	while (++i < data->n_p)
		pthread_create(&data->threads[i], NULL, philsopher, data->phils[i]);
	i = -1;
	while (++i < data->n_p)
		pthread_join(data->threads[i], NULL);
	pthread_join(monitor_thread, NULL);
	free_data(data);
	return (0);
}
