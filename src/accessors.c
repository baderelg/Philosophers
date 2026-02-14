/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:32:07 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:27:35 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Safe getter for meals_eaten
 *
 * Acquire the meal_lock to read the meals_eaten counter
 *
 * @param philo The philo structure
 * @return The number of meals eaten
 */
int	get_meals_eaten(t_philo *philo)
{
	int	meals;

	pthread_mutex_lock(&philo->meal_lock);
	meals = philo->meals_eaten;
	pthread_mutex_unlock(&philo->meal_lock);
	return (meals);
}

/**
 * @brief Safe getter for last_meal_time
 *
 * This is essential for the monitor to check for starvation without causing
 * any data races with the philo threads that update it
 *
 * @param philo The philo structure
 * @return The timestamp of the last meal
 */
long	get_last_meal_time(t_philo *philo)
{
	long	time;

	pthread_mutex_lock(&philo->meal_lock);
	time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_lock);
	return (time);
}

/**
 * @brief Safe setter for last_meal_time
 *
 * This is called immediately when a philo starts eating
 *
 * @param philo The philo structure
 * @param time The current timestamp
 */
void	set_last_meal_time(t_philo *philo, long time)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = time;
	pthread_mutex_unlock(&philo->meal_lock);
}

/**
 * @brief Safe incrementer for the meal counter
 *
 * This function is called when a philo finishes a meal
 *
 * @param philo The philo structure
 */
void	increment_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
}
