/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 14:25:21 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/12 23:27:57 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Validate the parsed arguments
 *
 * This ensures that time values are strictly positive & that the program has
 * at least 1 philo
 *
 * @param data The filled data structure to check
 * @return 0 if valid & 1 if a value is not
 */
static int	validate_args(t_data *data)
{
	if (data->num_philos < 1)
		return (1);
	if (data->time_to_die < 1)
		return (1);
	if (data->time_to_eat < 1)
		return (1);
	if (data->time_to_sleep < 1)
		return (1);
	if (data->must_eat_count == 0)
		return (1);
	return (0);
}

/**
 * @brief Parse received arguments & fill the data structure
 *
 * This function converts the string arguments to ints using a helper function
 * and handles the optional argument (number of times each philo must eat)
 *
 * The optional argument is initialized at -1 in case there is no need for it
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @param data Pointer to the data struct to fill
 * @return 0 on success & 1 on parsing error / invalid values
 */
int	parse_args(int argc, char **argv, t_data *data)
{
	data->must_eat_count = -1;
	if (parse_int(argv[1], &data->num_philos) != 0)
		return (1);
	if (parse_int(argv[2], &data->time_to_die) != 0)
		return (1);
	if (parse_int(argv[3], &data->time_to_eat) != 0)
		return (1);
	if (parse_int(argv[4], &data->time_to_sleep) != 0)
		return (1);
	if (argc == 6)
	{
		if (parse_int(argv[5], &data->must_eat_count) != 0)
			return (1);
	}
	return (validate_args(data));
}
