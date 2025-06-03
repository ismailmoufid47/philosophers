/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:54:52 by isel-mou          #+#    #+#             */
/*   Updated: 2025/06/03 17:38:11 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

/* ============ LIBRARIES ============ */
# include <semaphore.h>
# include <stdbool.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>

/* ========= SEMAPHORE NAMES ========= */
# define F "/forks"
# define L "/log_lock"
# define ML "/meal_lock"
# define ME "/must_eat"
# define D "/is_dead"

/* ========= CREATION FLAG =========== */
# define C O_CREAT

/* ========== ERROR MESSAGES ========= */
# define TIME "Time values"
# define N_MEALS "Number of Meals each philosopher must eat"
# define PHILO_MAX_COUNT_MSG "Philosopher count must be between 1 and 200\n"
# define ARGUMENT_OVERFLOW_MSG " must be between 0 and LONG_MAX\n"

/* ========== Type Definitions ========== */
typedef pthread_t			t_id;
typedef struct timeval		t_timeval;
typedef struct s_philo		t_philo;
typedef struct s_data		t_data;

/* ========== Data Structures ========== */
typedef struct s_sems
{
	sem_t					*forks;
	sem_t					*is_dead;
	sem_t					*log_lock;
	sem_t					*meal_lock;
	sem_t					*must_eat;
}	t_sems;

typedef struct s_history
{
	time_t					last_meal;
	long					meals_eaten;
}	t_history;

typedef struct s_times
{
	time_t					start;
	time_t					die;
	time_t					eat;
	time_t					sleep;
}	t_times;

struct s_philo
{
	int						id;
	pid_t					pid;
	t_history				history;
	t_data					*data;
};

struct s_data
{
	t_sems					sems;
	t_philo					*philos;
	t_times					time_to;
	long					meals_to_eat;
	int						philo_count;
};

/* ========== Initialization Function ========== */
void		init_data(t_data *data, t_philo *philos, char **argv);

/* ========== Simulation Function ========== */
void		launch_simulation(t_data *data);

/* ========== Utility Functions ========== */
time_t		get_current_time(void);
long		ft_atol(const char *str);
void		error_message(char *text, int signal);
void		destroy_all(t_data *data, char *str, int signal);
void		log_action(t_philo *philo, char *action);
void		ft_usleep(time_t mls);

/* ========== Monitor Utils Functions ========== */
void		check_philosopher_death(t_philo *philo);

/* ========== Simulation Status Functions ========== */
int			simulation_ended(t_data *data);
void		end_simulation(t_data *data);

#endif   /* PHILO_BONUS_H */
