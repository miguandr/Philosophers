/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 21:21:19 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/17 12:07:07 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * Assigns forks to each philosopher for the simulation.
 * @philos: Array of philosopher structures.
 * @forks: Array of mutexes representing the forks.
 * @num_philos: The total number of philosophers.
 *
 * This function assigns the right and left forks for each philosopher.
 * Each philosopher's right fork is the one corresponding to their index,
 * and the left fork is the next one in the array, looping back for the
 * last philosopher.
 */
static void	assign_forks(t_philo *philos, t_mtx *forks, int num_philos)
{
	int	i;

	i = 0;
	while (i < num_philos)
	{
		philos[i].right_fork = &forks[i];
		philos[i].left_fork = &forks[(i + 1) % num_philos];
		i++;
	}
}

/**
 * Initializes the philosopher structures for the simulation.
 * @philos: Array of philosopher structures.
 * @data: Pointer to the data structure containing simulation parameters.
 *
 * This function initializes the philosopher structures, setting their ID,
 * meal count, and the time of their last meal. It also initializes a mutex
 * for each philosopher.
 */
static void	init_philos(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		philos[i].data = data;
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].eating = 0;
		philos[i].last_meal = get_time();
		if (mutex_functions(&philos[i].philo_mtx, INIT) != 0)
			return ;
		i++;
	}
	assign_forks(philos, data->forks, data->num_philos);
}

/**
 * Initializes the input values for the simulation.
 * @data: Pointer to the data structure for the simulation.
 * @value: Array of input argument values.
 *
 * This function reads the input arguments and assigns them to the
 * corresponding fields in the data structure. It handles the number
 * of philosophers and timing parameters.
 */
static void	init_input(t_data *data, char **value)
{
	data->num_philos = ft_atol(value[1]);
	data->time_to_die = ft_atol(value[2]);
	data->time_to_eat = ft_atol(value[3]);
	data->time_to_sleep = ft_atol(value[4]);
	if (value[5])
		data->num_times_to_eat = ft_atol(value[5]);
	else
		data->num_times_to_eat = -1;
}

/**
 * Initializes the data structure and resources for the simulation.
 * @data: Pointer to the data structure for the simulation.
 * @forks: Array of mutexes representing forks.
 * @philos: Array of philosopher structures.
 * @value: Array of input argument values.
 *
 * This function initializes the philosopher, fork, and mutex structures,
 * as well as input parameters. It ensures that all necessary resources
 * are set up for the simulation.
 */
void	init_data(t_data *data, t_mtx *forks, t_philo *philos, char **value)
{
	int	i;

	data->philos = philos;
	data->forks = forks;
	data->end_simulation = false;
	data->ready_count = 0;
	data->dead_flag = 0;
	init_input(data, value);
	if ((mutex_functions(&data->mutex, INIT) != 0
			|| mutex_functions(&data->print_lock, INIT) != 0
			|| mutex_functions(&data->dead_lock, INIT) != 0))
		return ;
	i = 0;
	while (i < data->num_philos)
	{
		if (mutex_functions(&forks[i], INIT) != 0)
			return ;
		i++;
	}
	init_philos(philos, data);
}
