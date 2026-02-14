/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:28:50 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:27:08 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Wait for the main thread to signal the start of the sim
 *
 * This function spints until all_started is set to 1 by the main thread
 *
 * @param data Shared data structure
 */
static void	wait_for_start(t_data *data)
{
	while (1)
	{
		pthread_mutex_lock(&data->data_lock);
		if (data->all_started)
		{
			pthread_mutex_unlock(&data->data_lock);
			break ;
		}
		pthread_mutex_unlock(&data->data_lock);
		usleep(100);
	}
}

/**
 * @brief Handle the special case of a single philo
 *
 * A single philo takes their only fork but can't eat since they need two so
 * they just wait until they starve
 *
 * @param philo The philo structure
 */
static void	single_philo_case(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	print_status(philo, "has taken a fork");
	precise_wait(philo->data->time_to_die, philo->data);
	pthread_mutex_unlock(philo->first_fork);
}

/**
 * @brief The main lifecycle loop of a philo
 *
 * Cycling through eating, sleeping and thinking until the simulation stops
 *
 * @param philo The philo structure
 */
static void	philo_loop(t_philo *philo)
{
	while (!is_sim_stopped(philo->data))
	{
		philo_eat(philo);
		if (is_sim_stopped(philo->data))
			break ;
		philo_sleep(philo);
		if (is_sim_stopped(philo->data))
			break ;
		philo_think(philo, 0);
	}
}

/**
 * @brief Thread entry point for a philo
 *
 * 1. Signal readiness to main thread
 * 2. Wait for global start signal
 * 3. Handle the single philo edge case
 * 4. Stagger start time (even ID philos think first)
 * 5. Enter the main loop
 *
 * @param arg Void pointer to the philo structure
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->data_lock);
	philo->data->threads_ready++;
	pthread_mutex_unlock(&philo->data->data_lock);
	wait_for_start(philo->data);
	set_last_meal_time(philo, get_time_ms());
	if (philo->data->num_philos == 1)
	{
		single_philo_case(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		philo_think(philo, 1);
	philo_loop(philo);
	return (NULL);
}
