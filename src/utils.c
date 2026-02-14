/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:02:25 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:28:57 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Convert a string to an integer
 *
 * A custom atoi that checks for non numeric characters and overflow
 *
 * @param str The string to convert
 * @param result Pointer to store the result
 * @return 0 on success & 1 on invalid input or overflow
 */
int	parse_int(const char *str, int *result)
{
	long	num;
	int		i;

	num = 0;
	i = 0;
	if (!str || !str[0])
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		num = num * 10 + (str[i] - '0');
		if (num > 2147483647)
			return (1);
		i++;
	}
	*result = (int)num;
	return (0);
}

/**
 * @brief Print a status message
 *
 * This uses a double lock strategy:
 * 1. Lock data_lock to check if sim is stopped (early return)
 * 2. Lock print_lock
 * 3. Lock data_lock AGAIN to re-check sim_stop and calculate the timestamp
 *
 * The second check is needed because a philo could pass the first check, then
 * wait for the print lock.
 * If another thread prints "died" during that time & stops the simulation,
 * this prevents from printing a status message after the death one
 *
 * @param philo The philo structure
 * @param msg The status message to display
 */
void	print_status(t_philo *philo, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->data_lock);
	if (philo->data->sim_stop)
	{
		pthread_mutex_unlock(&philo->data->data_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->data->data_lock);
	pthread_mutex_lock(&philo->data->print_lock);
	pthread_mutex_lock(&philo->data->data_lock);
	if (!philo->data->sim_stop)
	{
		timestamp = get_time_ms() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->data_lock);
	pthread_mutex_unlock(&philo->data->print_lock);
}

/**
 * @brief Safe getter for the sim_stop flag
 *
 * @param data Shared data structure
 * @return 1 if simulation should stop & 0 otherwise
 */
int	is_sim_stopped(t_data *data)
{
	int	stopped;

	pthread_mutex_lock(&data->data_lock);
	stopped = data->sim_stop;
	pthread_mutex_unlock(&data->data_lock);
	return (stopped);
}

/**
 * @brief Safe setter for the sim_stop flag
 *
 * This function sets the flag to 1, signaling all threads to stop
 *
 * @param data Shared data structure
 */
void	set_sim_stop(t_data *data)
{
	pthread_mutex_lock(&data->data_lock);
	data->sim_stop = 1;
	pthread_mutex_unlock(&data->data_lock);
}
