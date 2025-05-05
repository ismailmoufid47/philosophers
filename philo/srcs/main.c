/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:58:47 by isel-mou          #+#    #+#             */
/*   Updated: 2025/05/05 12:54:19 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	validate_args(int argc, char **argv)
{
	int	i;

	i = 0;
	while (++i < argc)
	{
		if (i == 1 && (ft_atol(argv[i]) < 1 || ft_atol(argv[i]) > 200))
			error_message(PHILO_MAX_COUNT_MSG, 1);
		else if (i == 5)
		{
			if (ft_atol(argv[i]) < 0)
				error_message(N_MEALS ARGUMENT_OVERFLOW_MSG, 1);
			else if (ft_atol(argv[i]) == 0)
				exit(EXIT_SUCCESS);
		}
		else if (i != 1 && i != 5 && ft_atol(argv[i]) < 0)
			error_message(TIME ARGUMENT_OVERFLOW_MSG, 1);
	}
}

// ----------------------------------------------------------------------------
// Initialize philosopher instance
void	init_philos(t_data *data, char **argv)
{
	int			i;
	int			count;
	t_philo		*philo;

	count = ft_atol(argv[1]);
	i = -1;
	while (++i < count)
	{
		philo = &data->philos[i];
		philo->data = data;
		philo->id = i + 1;
		philo->history.meals_eaten = 0;
		philo->history.last_meal = data->time_to.start;
		philo->left_fork = &data->mutexes.forks[i];
		philo->right_fork = &data->mutexes.forks[(i + 1) % count];
	}
}

// ----------------------------------------------------------------------------
// Initialize data instance
void	init_data(t_data *data, t_philo *philos, t_mutex *forks, char **argv)
{
	int		i;

	data->philo_count = ft_atol(argv[1]);
	if (data->philo_count <= 0 || data->philo_count > 200)
		error_message("Error: Invalid philosopher count\n", 1);
	data->time_to.start = get_current_time();
	data->time_to.die = ft_atol(argv[2]);
	data->time_to.eat = ft_atol(argv[3]);
	data->time_to.sleep = ft_atol(argv[4]);
	data->must_eat = -1;
	if (argv[5])
		data->must_eat = ft_atol(argv[5]);
	data->mutexes.forks = forks;
	data->philos = philos;
	i = -1;
	while (++i < data->philo_count)
	{
		if (pthread_mutex_init(&data->mutexes.forks[i], NULL))
			destroy_all(data, "Mutex Init ERROR\n", 1);
	}
	init_philos(data, argv);
	if (pthread_mutex_init(&data->mutexes.log_lock, NULL)
		|| pthread_mutex_init(&data->mutexes.meal_lock, NULL)
		|| pthread_mutex_init(&data->mutexes.is_dead_lock, NULL))
		destroy_all(data, "Mutex Init ERROR\n", 1);
}

// ----------------------------------------------------------------------------
// Main function
int	main(int argc, char **argv)
{
	t_philo		philos[200];
	t_mutex		forks[200];
	t_data		data;

	if (argc < 5 || argc > 6)
		error_message(
			"Arguments must be in the form: "
			"./philo <number_of_phils> <time_to_die> <time_to_eat> "
			"optional arg: <number_of_times_each_philosopher_must_eat>\n", 1);
	validate_args(argc, argv);
	init_data(&data, philos, forks, argv);
	launch_simulation(&data);
	destroy_all(&data, NULL, 0);
	return (0);
}
