/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:53:55 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/24 17:31:49 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <limits.h>
# include <sys/time.h>

typedef pthread_mutex_t			t_mutex;
typedef pthread_t				t_thread;
typedef struct s_philosopher	t_philo;

typedef struct s_data
{
	unsigned long long	n_p;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	unsigned long long	number_to_eat;
	t_mutex				*p_lock;
	t_mutex				*forks;
	t_philo				**phils;
	t_thread			*threads;
	time_t				s_time;
	int					is_dead;
}	t_data;

// Philosopher structure
struct s_philosopher
{
	int					id;
	t_data				*data;
	time_t				last_meal;
	unsigned long long	n_meals;
};

void				think(t_philo *phil);
void				pick_up_forks(t_philo *phil);
void				eat(t_philo *phil);
void				put_down_forks(t_philo *phil);
void				sleep_philo(t_philo *phil);
time_t				time_ms(void);
void				log_action(t_philo *phil, const char *action);
void				*malloc_w(size_t size);
void				free_data(t_data *data);
unsigned long long	atoull(t_data *data, const char *str);

#endif