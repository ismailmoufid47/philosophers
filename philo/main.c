/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:10:12 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/26 21:33:07 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_data(t_data *dt, int argc, char **av)
{
	long	i;

	if (argc < 5 || argc > 6 || !ft_atl(av[1]) || !ft_atl(av[2])
		|| !ft_atl(av[3]) || !ft_atl(av[4]) || (argc == 6 && !ft_atl(av[5])))
	{
		free(dt);
		write(2, "Error: Invalid or Wrong number of arguments\n", 44);
		exit(1);
	}
	*dt = (t_data){ft_atl(av[1]), ft_atl(av[2]), ft_atl(av[3]), ft_atl(av[4]),
		.is_there_n_to_eat = 0, .s_time = time_ms(),
		.forks = malloc_w(sizeof(t_mutex) * ft_atl(av[1])),
		.phils = malloc_w(sizeof(t_philo *) * ft_atl(av[1])),
		.threads = malloc_w(sizeof(t_thread) * ft_atl(av[1])),
		.done = 0, .p_lock = malloc_w(sizeof(t_mutex))};
	if (argc == 6)
		dt->number_to_eat = ((dt->is_there_n_to_eat = 1), ft_atl(av[5]));
	pthread_mutex_init(dt->p_lock, NULL);
	i = 0;
	while (i < dt->n_p)
	{
		dt->phils[i] = malloc_w(sizeof(t_philo));
		*dt->phils[i] = (t_philo){i, dt, time_ms(), 0};
		pthread_mutex_init(&dt->forks[i++], NULL);
	}
}

void	*philosopher(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->id % 2 == 1)
		usleep(500);
	while (think(phil) && pick_up_forks(phil) && eat(phil) && sleep_philo(phil))
		;
	return (NULL);
}

int	check_full_philosophers(t_data *data)
{
	long	full_phils;
	long	i;

	full_phils = 0;
	i = 0;
	if (data->is_there_n_to_eat)
	{
		while (i < data->n_p)
		{
			if (data->phils[i]->n_meals >= data->number_to_eat)
				full_phils++;
			i++;
		}
		if (full_phils == data->n_p)
		{
			data->done = 1;
			printf("tama ta3bi2at lkirsh bi naja7\n");
			return (1);
		}
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_data	*data;
	long	i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->n_p)
		{
			if (time_ms() - data->phils[i]->last_meal > data->time_to_die)
			{
				log_action(data->phils[i], "died");
				data->done = 1;
				return (NULL);
			}
			i++;
		}
		if (check_full_philosophers(data))
			return (NULL);
	}
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_thread	monitor_thread;
	long		i;

	data = malloc_w(sizeof(t_data));
	init_data(data, argc, argv);
	pthread_create(&monitor_thread, NULL, monitor, data);
	i = 0;
	while (i < data->n_p)
	{
		pthread_create(&data->threads[i], NULL, philosopher, data->phils[i]);
		i++;
	}
	i = 0;
	while (i < data->n_p)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	free_data(data);
	return (0);
}
