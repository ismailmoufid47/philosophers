/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:10:12 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/25 16:46:10 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void check_leaks(void)
{
	system("leaks a.out");
}

void	init_data(t_data *data, int argc, char **av)
{
	int	i;

	if (argc < 5 || argc > 6 || !atoull(av[1]) || !atoull(av[2])
		|| !atoull(av[3]) || !atoull(av[4]) || (argc == 6 && !atoull(av[5])))
	{
		free(data);
		printf("Error: Invalid or Wrong number of arguments\n");
		exit(1);
	}
	*data = (t_data){atoull(av[1]), atoull(av[2]), atoull(av[3]), atoull(av[4]),
		.is_there_n_to_eat = 0, .s_time = time_ms(),
		.forks = malloc_w(sizeof(t_mutex) * atoull(av[1])),
		.phils = malloc_w(sizeof(t_philo *) * atoull(av[1])),
		.threads = malloc_w(sizeof(t_thread) * atoull(av[1])),
		.is_dead = 0, .p_lock = malloc_w(sizeof(t_mutex))};
	if (argc == 6)
		data->number_to_eat = ((data->is_there_n_to_eat = 1), atoull(av[5]));
	pthread_mutex_init(data->p_lock, NULL);
	i = -1;
	while (++i < data->n_p)
	{
		data->phils[i] = malloc_w(sizeof(t_philo));
		*data->phils[i] = (t_philo){i, data, time_ms(), 0};
		pthread_mutex_init(&data->forks[i], NULL);
	}
}

void	*philosopher(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->id % 2 == 0)
		usleep(500);
	while (think(phil) && pick_up_forks(phil) && eat(phil) && sleep_philo(phil))
		;
	return (NULL);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		full_phils;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->n_p)
		{
			if (time_ms() - data->phils[i]->last_meal > data->time_to_die)
			{
				log_action(data->phils[i], "died");
				data->is_dead = 1;
				return (NULL);
			}
			if (data->is_there_n_to_eat)
			{
				full_phils = 0;
				i = -1;
				while (++i < data->n_p)
					if (data->phils[i]->n_meals >= data->number_to_eat)
						full_phils++;
				if (full_phils == data->n_p)
				{
					printf("tama ta3bi2at lkirsh bi naja7\n");
					data->is_dead = 1;
					return (NULL);
				}
			}
		}
	}
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_thread	monitor_thread;
	int			i;

atexit(check_leaks);
	data = malloc_w(sizeof(t_data));
	init_data(data, argc, argv);
	i = -1;
	pthread_create(&monitor_thread, NULL, monitor, data);
	while (++i < data->n_p)
		pthread_create(&data->threads[i], NULL, philosopher, data->phils[i]);
	i = -1;
	while (++i < data->n_p)
		pthread_join(data->threads[i], NULL);
	pthread_join(monitor_thread, NULL);
	free_data(data);
	return (0);
}
