/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:58:47 by isel-mou          #+#    #+#             */
/*   Updated: 2025/05/07 16:44:08 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// ----------------------------------------------------------------------------
// Validate command line arguments
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
	}
}

// ----------------------------------------------------------------------------
// Initialize data instance
void	init_data(t_data *data, t_philo *philos, char **argv)
{
	data->philo_count = ft_atol(argv[1]);
	data->time_to.start = get_current_time();
	data->time_to.die = ft_atol(argv[2]);
	data->time_to.eat = ft_atol(argv[3]);
	data->time_to.sleep = ft_atol(argv[4]);
	data->philos = philos;
	init_philos(data, argv);
	data->sems.forks = (sem_unlink(F), sem_open(F, C, 0644, data->philo_count));
	data->sems.log_lock = (sem_unlink(L), sem_open(L, C, 0644, 1));
	data->sems.meal_lock = (sem_unlink(ML), sem_open(ML, C, 0644, 1));
	data->sems.is_dead = (sem_unlink(D), sem_open(D, C, 0644, 0));
	data->meals_to_eat = -1;
	data->sems.must_eat = NULL;
	if (argv[5])
	{
		data->sems.must_eat = (sem_unlink(ME), sem_open(ME, C, 0644, 0));
		data->meals_to_eat = ft_atol(argv[5]);
	}
	if (data->sems.forks == SEM_FAILED
		|| data->sems.log_lock == SEM_FAILED
		|| data->sems.meal_lock == SEM_FAILED
		|| data->sems.must_eat == SEM_FAILED
		|| data->sems.is_dead == SEM_FAILED)
		destroy_all(data, "Semaphore Init ERROR\n", 1);
}

// ----------------------------------------------------------------------------
// Main function
int	main(int argc, char **argv)
{
	t_philo		philos[200];
	t_data		data;

	if (argc < 5 || argc > 6)
		error_message(
			"Arguments must be in the form: "
			"./philo <number_of_phils> <time_to_die> <time_to_eat> "
			"optional arg: <number_of_times_each_philosopher_must_eat>\n", 1);
	validate_args(argc, argv);
	init_data(&data, philos, argv);
	launch_simulation(&data);
	destroy_all(&data, NULL, 0);
	return (0);
}
