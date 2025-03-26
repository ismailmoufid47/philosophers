/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:53:55 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/26 22:53:45 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef pthread_mutex_t			t_mutex;
typedef pthread_t				t_thread;
typedef struct s_philosopher	t_philo;

typedef struct s_data
{
	long						n_p;
	time_t						time_to_die;
	time_t						time_to_eat;
	time_t						time_to_sleep;
	int							is_there_n_to_eat;
	long						number_to_eat;
	t_mutex						*p_lock;
	t_mutex						*forks;
	t_philo						**phils;
	t_thread					*threads;
	time_t						s_time;
	int							done;
	t_mutex						*done_lock;
}	t_data;

// Philosopher structure
struct s_philosopher
{
	int							id;
	t_data						*data;
	time_t						last_meal;
	long						n_meals;
};

int								think(t_philo *phil);
int								pick_up_forks(t_philo *phil);
int								eat(t_philo *phil);
void								put_down_forks(t_philo *phil);
int								sleep_philo(t_philo *phil);

time_t							time_ms(void);
void							log_action(t_philo *phil, const char *action);
void							*malloc_w(size_t size);
void							free_data(t_data *data);
long							ft_atl(const char *str);

#endif