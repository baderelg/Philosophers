/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 13:55:44 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:28:22 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Assign fork pointers to a philo to prevent deadlocks
 *
 * This function implements a resource hierearchy (Djikstra solution)
 * - Even IDs pick the right fork first, then the left
 * - Odd IDs pick the left fork first
 * This breaks the circular wait (if all philos try to eat at the same time)
 *
 * @param philo The philo to assign forks to
 * @param data Shared data containing the fork array
 * @param id The index of the philo (starting 0)
 */
static void	assign_forks(t_philo *philo, t_data *data, int id)
{
	int	right;
	int	left;

	right = id;
	left = (id + 1) % data->num_philos;
	if (id % 2 == 0)
	{
		philo->first_fork = &data->forks[right];
		philo->second_fork = &data->forks[left];
	}
	else
	{
		philo->first_fork = &data->forks[left];
		philo->second_fork = &data->forks[right];
	}
}

/**
 * @brief Initialize a single philo structure
 *
 * Sets the philo ID (starting 1), the initial meal counters & initializes
 * the meal_lock mutex
 *
 * @param data Shared data structure
 * @param i Index of the philo (starting 0)
 * @return 0 on success & 1 on mutex initialization failure
 */
static int	init_single_philo(t_data *data, int i)
{
	t_philo	*philo;

	philo = &data->philos[i];
	philo->id = i + 1;
	philo->meals_eaten = 0;
	philo->last_meal_time = 0;
	philo->data = data;
	assign_forks(philo, data, i);
	if (pthread_mutex_init(&philo->meal_lock, NULL) != 0)
		return (1);
	return (0);
}

/**
 * @brief Initialize all philo structures
 *
 * This loops through the philo array & initializes one by one
 * If an initialization fails it cleans all mutexes to avoid leaks
 *
 * @param data Shared data structure
 * @return 0 on success & 1 on failure
 */
int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (init_single_philo(data, i) != 0)
		{
			destroy_mutexes(data, data->num_philos, i);
			return (1);
		}
		i++;
	}
	return (0);
}
