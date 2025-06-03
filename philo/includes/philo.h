/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:54:52 by isel-mou          #+#    #+#             */
/*   Updated: 2025/06/03 17:05:08 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* ============ LIBRARIES ============ */
# include <stdbool.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

/* ============ EXIT CODES ============ */
# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

/* =========== ERROR MESSAGES =========== */
# define TIME "Time values"
# define N_MEALS "Number of Meals each philosopher must eat"
# define PHILO_MAX_COUNT_MSG "Philosopher count must be between 1 and 200\n"
# define ARGUMENT_OVERFLOW_MSG " must be between 0 and LONG_MAX\n"

/* ========== Type Definitions ========== */
typedef pthread_t			t_id;
typedef pthread_mutex_t		t_mutex;
typedef struct timeval		t_timeval;
typedef struct s_philo		t_philo;
typedef struct s_data		t_data;

/* ========== Data Structures ========== */
typedef struct s_mutexes
{
	t_mutex					*forks;
	t_mutex					log_lock;
	t_mutex					meal_lock;
	t_mutex					is_dead_lock;
}	t_mutexes;

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
	t_id					thread_id;
	t_history				history;
	t_mutex					*left_fork;
	t_mutex					*right_fork;
	t_data					*data;
};

struct s_data
{
	t_mutexes				mutexes;
	t_philo					*philos;
	t_times					time_to;
	long					must_eat;
	int						philo_count;
	int						is_dead;
};

/* ========== Initialization Function ========== */
int			init_data(t_data *data, t_philo *philos,
				t_mutex *forks, char **argv);

/* ========== Simulation Function ========== */
int			launch_simulation(t_data *data);

/* ========== Utility Functions ========== */
time_t		get_current_time(void);
long		ft_atol(const char *str);
void		error_message(char *text);
void		destroy_all(t_data *data, char *str);
void		log_action(t_philo *philo, char *action);
void		ft_usleep(t_philo *philo, time_t mls);

/* ========== Monitor Utils Functions ========== */
bool		are_you_full(t_philo *philos);
bool		check_philosopher_death(t_philo *philo);
void		report_death(t_philo *philo);
bool		are_you_dead(t_philo *philos);

/* ========== Simulation Status Functions ========== */
int			simulation_ended(t_data *data);
void		end_simulation(t_data *data);

#endif   /* PHILO_H */
