/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:53:55 by isel-mou          #+#    #+#             */
/*   Updated: 2025/03/21 22:02:08 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

# define PHILOSOPHERS 5
# define TIME_TO_DIE 1   // 5 seconds
# define TIME_TO_EAT 1000   // 1 second
# define TIME_TO_SLEEP 2000 // 2 seconds

typedef pthread_mutex_t			t_mutex;
typedef pthread_t				t_thread;
typedef struct s_philosopher	t_philo;
typedef long long				t_time;

typedef struct s_data
{
	t_mutex			*p_lock;
	t_mutex			*forks;
	t_philo			**phils;
	t_thread		*threads;
	int				is_dead;
	t_time			s_time;
}	t_data;

// Philosopher structure
struct s_philosopher
{
	int				id;
	t_data			*data;
	t_time			last_meal; 
	t_mutex			*meal_lock;
};


void	think(t_philo *philo);
void	pick_up_forks(t_philo *philo);
void	eat(t_philo *philo);
void	put_down_forks(t_philo *philo);
void	sleep_philo(t_philo *philo);
t_time	time_ms(void);
void	log_action(t_philo *philo, const char *action);
void	*malloc_w(size_t size);
void	free_data(t_data *data);

#endif