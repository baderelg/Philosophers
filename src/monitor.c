/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 11:55:32 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:28:01 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Check if a philo has starved to death
 *
 * This function calculates the time elapsed since the last meal.
 * If it exceeds time_to_die then the philo is considered as dead
 *
 * @param philo The philo to check
 * @param current_time The current timestamp
 * @return 1 if the philo is dead, 0 if still alive
 */
static int	philo_is_dead(t_philo *philo, long current_time)
{
	long	last_meal;
	long	time_since_meal;

	last_meal = get_last_meal_time(philo);
	if (last_meal == 0)
		return (0);
	time_since_meal = current_time - last_meal;
	if (time_since_meal > philo->data->time_to_die)
		return (1);
	return (0);
}

/**
 * @brief Iterate through all philos to check for death
 *
 * If a death is detected, the global stop flag is set and it prints the
 * death message (protected by the print mutex)
 *
 * @param data Shared data structure
 * @return 1 if a death occured and 0 otherwise
 */
static int	check_deaths(t_data *data)
{
	int		i;
	long	current_time;

	current_time = get_time_ms();
	i = 0;
	while (i < data->num_philos)
	{
		if (philo_is_dead(&data->philos[i], current_time))
		{
			set_sim_stop(data);
			pthread_mutex_lock(&data->print_lock);
			printf("%ld %d died\n",
				current_time - data->start_time, data->philos[i].id);
			pthread_mutex_unlock(&data->print_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Check if all philos have eaten enough meals
 *
 * Only runs if the must_eat_count argument was provided
 *
 * @param data Shared data structure
 * @return 1 if every philo has reached the target, 0 otherwise
 */
static int	all_ate_enough(t_data *data)
{
	int	i;
	int	finished;

	if (data->must_eat_count == -1)
		return (0);
	finished = 0;
	i = 0;
	while (i < data->num_philos)
	{
		if (get_meals_eaten(&data->philos[i]) >= data->must_eat_count)
			finished++;
		i++;
	}
	if (finished == data->num_philos)
		return (1);
	return (0);
}

/**
 * @brief Join all philos threads
 *
 * This function is called after the simulation is stopped, to make sure that
 * all threads have exited before the program terminates
 *
 * @param data Shared data structure
 */
static void	join_all_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

/**
 * @brief Main monitor loop
 *
 * - This function continously checks for deaths / meal completion
 * - It sleeps for a short interval (1 ms) between the checks
 * - It stops the simulation and joins the threads when any condition is met
 *
 * @param data Shared data structure
 */
void	monitor_philos(t_data *data)
{
	while (1)
	{
		if (is_sim_stopped(data))
			break ;
		if (check_deaths(data))
			break ;
		if (all_ate_enough(data))
		{
			set_sim_stop(data);
			break ;
		}
		usleep(1000);
	}
	join_all_threads(data);
}
