/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:10:12 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/24 18:01:00 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_data(t_data *data, int argc, char **av)
{
	int	i;

	if (argc < 5 || argc > 6 || !atoull(av[1]) || !atoull(av[2])
		|| !atoull(av[3]) || !atoull(av[4]) || (argc == 6 && !atoull(av[5])))
	{
		printf("Error: Invalid or Wrong number of arguments\n");
		exit(1);
	}
	*data = (t_data){
		atoull(av[1]), atoull(av[2]),
		atoull(av[3]), atoull(av[4]),
		.number_to_eat = atoull(av[5]), .s_time = time_ms(),
		.forks = malloc_w(sizeof(t_mutex) * atoull(av[1])),
		.phils = malloc_w(sizeof(t_philo *) * atoull(av[1])),
		.threads = malloc_w(sizeof(t_thread) * atoull(av[1])),
		.is_dead = 0, .p_lock = malloc_w(sizeof(t_mutex))};
	if (argc == 6)
		data->number_to_eat = atoull(av[5]);
	pthread_mutex_init(data->p_lock, NULL);
	i = -1;
	while (++i < data->n_p)
	{
		data->phils[i] = malloc_w(sizeof(t_philo));
		*data->phils[i] = (t_philo){i, data, time_ms(), 0};
		pthread_mutex_init(&data->forks[i], NULL);
	}
}

void	*philsopher(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->id % 2 == 0)
		usleep(500);
	while (1)
	{
		if (phil->data->is_dead)
			return (NULL);
		think(phil);
		pick_up_forks(phil);
		eat(phil);
		put_down_forks(phil);
		sleep_philo(phil);
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
			if (time_ms() - data->phils[i]->last_meal > data->time_to_die)
			{
				log_action(data->phils[i], "died");
				pthread_mutex_lock(data->p_lock);
				data->is_dead = 1;
				pthread_mutex_unlock(data->p_lock);
				return (NULL);
			}
		}
	}
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_thread	monitor_thread;
	int			i;

	data = malloc_w(sizeof(t_data));
	init_data(data, argc, argv);
	printf("number of philosophers: %llu\n \
		time to die: %ld\ntime to eat: %ld\ntime to sleep: %ld\n",
		data->n_p, data->time_to_die, data->time_to_eat, data->time_to_sleep);
	if (argc == 6)
		printf("number of times each philosopher must eat: %llu\n",
			data->number_to_eat);
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
