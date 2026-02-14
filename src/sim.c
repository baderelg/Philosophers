/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 19:22:02 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:27:50 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Create a thread for each philo
 *
 * Loops through the philo array & creates a thread for each\n
 * If creation fails at any time, it sets the sim_stop flag to stop the
 * simulation immediately
 *
 * @param data Shared data structure
 * @return 0 on success & 1 on failure during creation of a thread
 */
static int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]) != 0)
		{
			set_sim_stop(data);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Wait until all threads are ready
 *
 * This function acts as a barrier\n
 * The main thread spins until the threads_ready counter = the nbr of philos
 * ensuring that all the threads are alive & waiting before starting the timer
 *
 * @param data Shared data structure
 */
static void	wait_threads_ready(t_data *data)
{
	while (1)
	{
		pthread_mutex_lock(&data->data_lock);
		if (data->threads_ready == data->num_philos)
		{
			pthread_mutex_unlock(&data->data_lock);
			break ;
		}
		pthread_mutex_unlock(&data->data_lock);
		usleep(100);
	}
}

/**
 * @brief Handle the start of the simulation
 *
 * 1. Create threads
 * 2. Wait for all threads to be ready
 * 3. Set the start timestamp
 * 4. Set up the all_started flag to release all threads
 *
 * @param data Shared data structure
 * @return 0 on success & 1 on failure
 */
int	start_simulation(t_data *data)
{
	if (create_threads(data) != 0)
		return (1);
	wait_threads_ready(data);
	data->start_time = get_time_ms();
	pthread_mutex_lock(&data->data_lock);
	data->all_started = 1;
	pthread_mutex_unlock(&data->data_lock);
	return (0);
}
