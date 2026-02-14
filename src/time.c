/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 19:37:11 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:41:28 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Get the current time
 *
 * This uses gettimeofday() in order to get the current time with seconds &
 * microseconds then convert it to milliseconds
 *
 * @return Current timestamp in milliseconds
 */
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

/**
 * @brief Pause execution with precise timing
 *
 * Instead of using a long usleep(), this function sleeps in smaller chunks for
 * 2 reasons:
 * - Precision (usleep() can be inaccurate for long durations)
 * - Responsiveness (frequent checks ensure threads exit immediately rather
 * than finishing a full sleep cycle)
 *
 * @param ms Duration to wait (in milliseconds)
 * @param data Shared data stucture (to check stop flag)
 */
void	precise_wait(long ms, t_data *data)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time_ms();
	while (!is_sim_stopped(data))
	{
		elapsed = get_time_ms() - start;
		if (elapsed >= ms)
			break ;
		remaining = ms - elapsed;
		if (remaining > 10)
			usleep(remaining * 500);
		else if (remaining > 1)
			usleep(500);
		else
			usleep(100);
	}
}
