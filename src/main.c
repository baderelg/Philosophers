/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 13:40:55 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:28:04 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Write an error message to STDERR
 *
 * This function is used instead of printf so that errors are written to fd 2
 *
 * @param msg The error message string to display
 * @return Always 1 (exit failure code)
 */
static int	print_error(char *msg)
{
	int	i;

	write(2, "Error: ", 7);
	i = 0;
	while (msg[i])
		i++;
	write(2, msg, i);
	write(2, "\n", 1);
	return (1);
}

/**
 * @brief Initialize the simulation
 *
 * This initializes the shared data struct and the philosopher threads
 *
 * @param data Pointer to the main data struct
 * @return 0 on success & 1 on allocation/mutex failure
 */
static int	init_all(t_data *data)
{
	if (init_data(data) != 0)
		return (1);
	if (init_philos(data) != 0)
		return (1);
	return (0);
}

/**
 * @brief Main function
 *
 * Validates arguments, initializes what is needed, starts the simulation
 * threads & hands over control to the monitor loop, then frees all resources
 * on exit
 *
 * @return 0 on success & 1 on error
 */
int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (argc < 5 || argc > 6)
		return (print_error("Invalid argument count"));
	if (parse_args(argc, argv, &data) != 0)
		return (print_error("Invalid arguments"));
	if (init_all(&data) != 0)
	{
		cleanup_data(&data);
		return (print_error("Initialization failed"));
	}
	if (start_simulation(&data) != 0)
	{
		cleanup_data(&data);
		return (print_error("Simulation failed"));
	}
	monitor_philos(&data);
	cleanup_data(&data);
	return (0);
}
