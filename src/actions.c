/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:28:39 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:28:54 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Lock the 2 forks associated with the philo
 *
 * (Dijkstra's Solution)
 * The order of locking is determined during initialization, to prevent deadlock
 *
 * @param philo The philo structure
 */
static void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->second_fork);
	print_status(philo, "has taken a fork");
}

/**
 * @brief Unlock the 2 forks held by the philo
 *
 * The order of unlocking is the reverse from locking, even though unlocking
 * order doesn't really affect deadlock safety, just performance and fairness
 *
 * @param philo The philo structure
 */
static void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->second_fork);
	pthread_mutex_unlock(philo->first_fork);
}

/**
 * @brief Handle the eating action
 *
 * 1. Take forks
 * 2. Update the last meal immediately
 * 3. Sleep for time_to_eat
 * 4. Increment meal counter (in case it's needed)
 * 5. Release forks
 *
 * @param philo The philo structure
 */
void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	set_last_meal_time(philo, get_time_ms());
	print_status(philo, "is eating");
	precise_wait(philo->data->time_to_eat, philo->data);
	increment_meals(philo);
	release_forks(philo);
}

/**
 * @brief Handle the sleeping action
 *
 * This prints the status and sleeps for time_to_sleep
 *
 * @param philo The philo structure
 */
void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_wait(philo->data->time_to_sleep, philo->data);
}

/**
 * @brief Handle the thinking action (with smart waiting)
 *
 * If we are at the start of the simulation, even numbered philos wait for a
 * bit to desync from odd ones
 *
 * Otherwise, this calculates a safe think time based on time_to_die where
 * the wait gets clamped between 0 and time_to_eat, to make sure that the philo
 * wakes up regularly (but not too much) to check if forks are free to take
 *
 * @param philo The philo structure
 * @param is_start Flag indicating if this is the start of the simulation
 */
void	philo_think(t_philo *philo, int is_start)
{
	long	think_time;
	long	time_since_meal;
	t_data	*data;

	data = philo->data;
	print_status(philo, "is thinking");
	if (is_start && philo->id % 2 == 0)
	{
		usleep(1000);
		return ;
	}
	time_since_meal = get_time_ms() - get_last_meal_time(philo);
	think_time = (data->time_to_die - time_since_meal - data->time_to_eat) / 2;
	if (think_time < 0)
		think_time = 0;
	if (think_time > data->time_to_eat)
		think_time = data->time_to_eat;
	if (think_time > 0)
		precise_wait(think_time, data);
}
