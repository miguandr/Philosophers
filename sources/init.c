/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 21:21:19 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/10 19:10:13 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*Philosophers start grabbing first the lowest numbered fork (right), and then
the highest numbered fork (left). This prevents deadlock and simultaneous waiting
of all the philosophers.*/
static void	assign_forks(t_philo *philos, t_forks *forks, int num_philos)
{
	int	i;
	int	left_i;
	int	right_i;

	i = 0;
	while (i < num_philos)
	{
		right_i = i;
		left_i = (i + 1) % num_philos;
		if (right_i < left_i)
		{
			philos[i].right_fork = &forks[right_i].fork;
			philos[i].left_fork = &forks[left_i].fork;
		}
		else
		{
			philos[i].right_fork = &forks[left_i].fork;
			philos[i].left_fork = &forks[right_i].fork;
		}
		i++;
	}
}

static void	init_philos(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		philos[i].data = data;
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = get_time();
		philos[i].full = false;
		if (mutex_functions(&philos[i].philo_mtx, INIT) != 0)
			return ;
		i++;
	}
	assign_forks(philos, data->forks, data->num_philos);
}

static void	init_input(t_data *data, char **value)
{
	data->num_philos = ft_atol(value[1]);
	data->time_to_die = ft_atol(value[2]); // * 1000 conversion to miliseconds (usleep)
	data->time_to_eat = ft_atol(value[3]);
	data->time_to_sleep = ft_atol(value[4]);
	if (value[5])
		data->num_times_to_eat = ft_atol(value[5]);
	else
		data->num_times_to_eat = -1;
}

void	init_data(t_data *data, t_forks *forks, t_philo *philos, char **value)
{
	int	i;

	data->philos = philos;
	data->forks = forks;
	data->dead_flag = 0;
	data->end_simulation = false;
	data->ready_count = 0;
	init_input(data, value);
	if ((mutex_functions(&data->mutex, INIT) != 0
			|| mutex_functions(&data->print_lock, INIT) != 0))
		return ;
	i = 0;
	while (i < data->num_philos)
	{
		if (mutex_functions(&forks[i].fork, INIT) != 0)
			return ;
		forks[i].id = i; // needed maybe for debbuging?
		i++;
	}
	init_philos(philos, data);
}
