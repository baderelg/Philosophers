/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 11:17:05 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:28:34 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Destroy mutexes
 *
 * This function is used during initialization failure.
 * If initialization fails at any point, it allows destroying only the mutexes
 * that were initialized up to that point
 *
 * @param data Shared data structure
 * @param fork_count Number of fork mutexes to destroy
 * @param philo_count Number of philo meal mutexes to destroy
 */
void	destroy_mutexes(t_data *data, int fork_count, int philo_count)
{
	int	i;

	i = 0;
	while (i < fork_count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	i = 0;
	while (i < philo_count)
	{
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		i++;
	}
}

/**
 * @brief Destroy all meal mutexes in the philos structures
 *
 * @param data Shared data structure
 */
static void	cleanup_philos(t_data *data)
{
	int	i;

	if (!data->philos)
		return ;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		i++;
	}
}

/**
 * @brief Destroy all fork mutexes
 *
 * @param data Shared data structure
 */
static void	cleanup_forks(t_data *data)
{
	int	i;

	if (!data->forks)
		return ;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}

/**
 * @brief Free all allocated memory & destroy mutexes
 *
 * This function is the final step of the program (or result of an error).
 * It makes sure the program exits with no memory leaks or locked resources
 *
 * @param data Shared data structure
 */
void	cleanup_data(t_data *data)
{
	cleanup_philos(data);
	cleanup_forks(data);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->data_lock);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}
