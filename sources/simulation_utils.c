/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 22:28:42 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/17 15:57:35 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * Custom usleep function that pauses execution for a specified number
 * of milliseconds.
 * @milliseconds: The number of milliseconds to sleep.
 *
 * This function repeatedly checks the current time and calls usleep in small
 * intervals until the specified time has elapsed.
 */
int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

/**
 * Gets the current time in milliseconds.
 *
 * This function retrieves the current time using gettimeofday, converts
 * it to milliseconds, and returns the result. If gettimeofday fails,
 * an error is raised.
 */
size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_error(7);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/**
 * Prints the status of a philosopher to the console.
 * @id: The ID of the philosopher.
 * @str: The status message to print.
 * @data: Pointer to the data structure holding simulation information.
 *
 * This function locks the print mutex, retrieves the current time since the
 * simulation started, and prints the philosopher's status. It then unlocks
 * the print mutex.
 */
void	print_status(int id, char *str, t_data *data)
{
	size_t	time;

	mutex_functions(&data->print_lock, LOCK);
	time = get_time() - data->start_simulation;
	if (!is_dead(data))
		printf("%zu %d %s\n", time, id, str);
	mutex_functions(&data->print_lock, UNLOCK);
}
