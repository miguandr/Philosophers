/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 22:28:42 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/10 17:56:55 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*void	ft_usleep() // needed???????
{
	;
}*/

int	ft_destroy(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	status = mutex_functions(&data->print_lock, DESTROY);
	if (status != 0)
		return (status);
	status = mutex_functions(&data->mutex, DESTROY);
	if (status != 0)
		return (status);
	while (i < data->num_philos)
	{
		status = mutex_functions(&data->forks->fork, DESTROY);
		if (status != 0)
			return (status);
		i++;
	}
	return (0);
}

void	print_status(int id, char *str, t_data *data) // Add mutex if used by more threads
{
	size_t	time;

	mutex_functions(&data->print_lock, LOCK);
	time = get_time() - data->start_simmulation;
	//if (!data->end_simulation)
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
