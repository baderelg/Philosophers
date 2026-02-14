/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 13:42:33 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:22:57 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

/**
 * @brief Shared simulation data
 *
 * This structure holds the parsed arguments & the shared state (mutexes and
 * simulation flags) accessed by all the philosophers
 */
typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				sim_stop;
	int				all_started;
	int				threads_ready;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	data_lock;
	struct s_philo	*philos;
}	t_data;

/**
 * @brief The philosopher state
 *
 * Contains the unique ID, number of meals eaten, timestamp for the last meal,
 * the thread handle and pointers to specific resources that this philosopher
 * will need.
 */
typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	pthread_mutex_t	meal_lock;
	t_data			*data;
}	t_philo;

int		parse_args(int argc, char **argv, t_data *data);

int		init_data(t_data *data);

int		init_philos(t_data *data);

int		start_simulation(t_data *data);

void	*philo_routine(void *arg);

void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo, int is_start);

void	monitor_philos(t_data *data);

long	get_time_ms(void);
void	precise_wait(long ms, t_data *data);

int		parse_int(const char *str, int *result);
void	print_status(t_philo *philo, char *msg);
int		is_sim_stopped(t_data *data);
void	set_sim_stop(t_data *data);

int		get_meals_eaten(t_philo *philo);
long	get_last_meal_time(t_philo *philo);
void	set_last_meal_time(t_philo *philo, long time);
void	increment_meals(t_philo *philo);

void	cleanup_data(t_data *data);
void	destroy_mutexes(t_data *data, int fork_count, int philo_count);

#endif
