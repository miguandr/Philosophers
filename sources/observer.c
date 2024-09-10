/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 23:20:55 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/10 18:09:03 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	philos_full(t_data *data)
{
	int	i;

	i = 0;
	if (data->num_times_to_eat == -1)
		return (false);
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->num_times_to_eat)
			return (false);
		i++;
	}
	printf("All philos have eaten!\n"); // DELETE?
	return (true);
}

static void	philos_dead(t_data *data, t_philo *philo)
{
	size_t	current_time;

	current_time = get_time();
	//pthread_mutex_lock(&philo->philo_mtx); // NEW
	if ((current_time - philo->last_meal) > data->time_to_die)
	{
		data->end_simulation = true;
		data->dead_flag = true;
		print_status(philo->id, "died", data);
	//	pthread_mutex_unlock(&philo->philo_mtx); // NEW
	}
	//pthread_mutex_unlock(&philo->philo_mtx); // NEW
}

void	*ft_observer(void *pointer)
{
	t_data	*data;
	t_philo	*philo;
	int		i;

	data = (t_data *)pointer;
	while (!data->end_simulation)
	{
		i = 0;
		while (i < data->num_philos)
		{
			philo = &data->philos[i];
			mutex_functions(&data->mutex, LOCK);
			philos_dead(data, philo);
			mutex_functions(&data->mutex, UNLOCK);
			if (data->end_simulation)
				break ;
			i++;
		}
		mutex_functions(&data->mutex, LOCK);
		if (philos_full(data))
			data->end_simulation = true;
		mutex_functions(&data->mutex, UNLOCK);
		usleep(100);
	}
	return (pointer);
}
