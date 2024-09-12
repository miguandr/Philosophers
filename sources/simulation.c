/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:02:27 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/12 20:34:11 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * The action a philosopher takes when eating.
 * @data: Pointer to the data structure holding simulation information.
 * @philo: Pointer to the philosopher performing the action.
 *
 * The philosopher locks the right fork first, and if they are the only one,
 * waits for the time to die and declares death. Otherwise, they lock the
 * left fork, update the last meal time, and increment their meals eaten counter.
 * After eating for the required time, the forks are unlocked.
 */
static void	ft_eat(t_data *data, t_philo *philo)
{
	mutex_functions(philo->right_fork, LOCK);
	print_status(philo->id, "has taken a fork", data);
	if (data->num_philos == 1)
	{
		ft_usleep(data->time_to_die);
		print_status(philo->id, "died", data);
		mutex_functions(&data->mutex, LOCK);
		data->end_simulation = true;
		mutex_functions(&data->mutex, UNLOCK);
		mutex_functions(philo->right_fork, UNLOCK);
		return ;
	}
	mutex_functions(philo->left_fork, LOCK);
	print_status(philo->id, "has taken a fork", data);
	mutex_functions(&philo->philo_mtx, LOCK);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	mutex_functions(&philo->philo_mtx, UNLOCK);
	print_status(philo->id, "is eating", data);
	ft_usleep(data->time_to_eat);
	mutex_functions(philo->left_fork, UNLOCK);
	mutex_functions(philo->right_fork, UNLOCK);
}

/**
 * Routine for each philosopher, which consists of eating, sleeping
 * and thinking.
 * @data: Pointer to the data structure holding simulation information.
 * @philo: Pointer to the philosopher performing the routine.
 *
 * The philosopher continuously performs the routine as long as the simulation
 * is running. They eat, sleep, and then think, with respective delays for each
 * action.
 */
static void	philos_routine(t_data *data, t_philo *philo)
{
	while (!end_simulation(data))
	{
		ft_eat(data, philo);
		if (end_simulation(data))
			break ;
		print_status(philo->id, "is sleeping", philo->data);
		if (end_simulation(data))
			break ;
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo->id, "is thinking", philo->data);
	}
}

/**
 * Simulation for each philosopher thread.
 * @pointer: A pointer to the philosopher passed to the thread.
 *
 * Each philosopher thread waits until all philosophers are ready, records
 * the start time, and initiates its routine. Even-numbered philosophers
 * wait for half the eating time before starting to avoid deadlocks.
 */
static void	*dinner_simulation(void *pointer)
{
	t_philo	*philos;
	t_data	*data;

	philos = (t_philo *)pointer;
	data = philos->data;
	set_ready_count(data);
	while (!get_philos_ready(data))
		ft_usleep(1);
	mutex_functions(&data->mutex, LOCK);
	data->start_simmulation = get_time();
	mutex_functions(&data->mutex, UNLOCK);

	mutex_functions(&data->mutex, LOCK);
	mutex_functions(&philos->philo_mtx, LOCK);
	philos->last_meal = data->start_simmulation;
	mutex_functions(&philos->philo_mtx, UNLOCK);
	mutex_functions(&data->mutex, UNLOCK);

	if (philos->id % 2 == 0)
		ft_usleep(data->time_to_eat / 2);
	while (!data->end_simulation)
		philos_routine(data, philos);
	return (NULL);
}

/**
 * Starts the philosopher threads for the simulation.
 * @data: Pointer to the data structure holding simulation information.
 *
 * This function creates a thread for each philosopher and starts the
 * dining simulation.
 * If thread creation fails, the function terminates and cleans up resources.
 */
static void	start_philos(t_data *data)
{
	int			i;
	int			status;

	i = 0;
	while (i < data->num_philos)
	{
		status = pthread_create(&data->philos[i].thread,
				NULL, dinner_simulation, &data->philos[i]);
		if (status != 0)
		{
			ft_destroy(data);
			return ;
		}
		i++;
	}
}

/**
 * Main program logic to start the simulation and join threads.
 * @data: Pointer to the data structure holding simulation information.
 *
 * This function starts the observer thread to monitor the simulation and
 * philosopher threads to run the dining simulation. It waits for all threads
 * to finish before exiting.
 */
void	start_program(t_data *data)
{
	pthread_t	observer;
	int			i;

	i = 0;
	if (data->num_times_to_eat == 0)
		return ;
	if (pthread_create(&observer, NULL, ft_observer, &data->philos))
	{
		ft_destroy(data);
		return ;
	}
	start_philos(data);
	while (i < data->num_philos)
	{
		if (thread_funtions(&data->philos[i].thread, NULL,
				NULL, JOIN) != 0)
			return ;
		i++;
	}
	if (thread_funtions(&observer, NULL, NULL, JOIN) != 0)
		return ;
}
