/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 22:28:42 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/10 19:48:50 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

int	ft_destroy(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->num_philos)
	{
		status = mutex_functions(&data->forks[i].fork, DESTROY);
		if (status != 0)
			return (status);
		i++;
	}
	status = mutex_functions(&data->print_lock, DESTROY);
	if (status != 0)
		return (status);
	status = mutex_functions(&data->mutex, DESTROY);
	if (status != 0)
		return (status);
	return (0);
}

void	print_status(int id, char *str, t_data *data)
{
	size_t	time;

	mutex_functions(&data->print_lock, LOCK);
	time = get_time() - data->start_simmulation;
	if (!data->end_simulation)
		printf("%zu: philo %d %s\n", time, id, str);
	mutex_functions(&data->print_lock, UNLOCK);
}

void	set_ready_count(t_data *data)
{
	mutex_functions(&data->mutex, LOCK); // Lock the data's mutex to ensure exclusive access.
	data->ready_count++;
	mutex_functions(&data->mutex, UNLOCK);
}

bool	get_philos_ready(t_data *data)
{
	int	ready_count;

	mutex_functions(&data->mutex, LOCK);
	ready_count = data->ready_count;
	mutex_functions(&data->mutex, UNLOCK);
	return (ready_count == data->num_philos);
}

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_error(7);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
