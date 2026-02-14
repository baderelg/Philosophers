/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 13:52:34 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:28:11 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initialize the fork mutexes
 *
 * This function loops through the nbr of philos & initializes a mutex for
 * each fork. When an initialization fails, it destroys the initialized
 * mutexes to prevent resource leaks
 *
 * @param data Pointer to the shared data struct
 * @return 0 on success & 1 on failure
 */
static int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			destroy_mutexes(data, i, 0);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Initialize global mutexes
 *
 * - print_lock is used for console output
 * - data_lock is used for shared flags like sim_stop
 *
 * @param data Pointer to the shared data struct
 * @return 0 on success & 1 on failure
 */
static int	init_global_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->data_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief Data initialization function
 *
 * This sets initial simulation flags and allocates memory for forks & philos
 * arrays then calls helper functions to initialize mutexes
 *
 * @param data Pointer to the shared data struct
 * @return 0 on success & 1 on allocation/mutex initialization failure
 */
int	init_data(t_data *data)
{
	data->sim_stop = 0;
	data->all_started = 0;
	data->threads_ready = 0;
	data->start_time = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (1);
	if (init_forks(data) != 0)
		return (1);
	if (init_global_mutexes(data) != 0)
	{
		destroy_mutexes(data, data->num_philos, 0);
		return (1);
	}
	return (0);
}
