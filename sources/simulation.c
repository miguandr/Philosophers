/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:02:27 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/17 12:41:35 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	is_dead(t_data *data) // NEW
{
	mutex_functions(&data->dead_lock, LOCK);
	if (data->dead_flag == 1)
	{
		mutex_functions(&data->dead_lock, UNLOCK);
		return (1);
	}
	mutex_functions(&data->dead_lock, UNLOCK);
	return (0);
}

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
		mutex_functions(philo->right_fork, UNLOCK);
		return ;
	}
	mutex_functions(philo->left_fork, LOCK);
	print_status(philo->id, "has taken a fork", data);
	philo->eating = 1;
	print_status(philo->id, "is eating", data);
	mutex_functions(&philo->philo_mtx, LOCK);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	mutex_functions(&philo->philo_mtx, UNLOCK);

	ft_usleep(data->time_to_eat);
	philo->eating = 0;
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
/*static void	philos_routine(t_data *data, t_philo *philo)
{
	while (!end_simulation(data))
	{
		ft_eat(data, philo);
		print_status(philo->id, "is sleeping", philo->data);
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo->id, "is thinking", philo->data);
	}
}*/

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

	mutex_functions(&philos->philo_mtx, LOCK);
	philos->philo_start = get_time();
	philos->last_meal = philos->philo_start;
	mutex_functions(&philos->philo_mtx, UNLOCK);

	if (philos->id % 2 == 0)
		ft_usleep(data->time_to_eat / 2);
	while (!is_dead(data))
	{
		ft_eat(data, philos);
		if (is_dead(data))
			break ;
		print_status(philos->id, "is sleeping", philos->data);
		ft_usleep(philos->data->time_to_sleep);
		print_status(philos->id, "is thinking", philos->data);
	}
	return (pointer);
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
	data->start_simulation = get_time();
	if (thread_funtions(&observer, ft_observer, &data->philos, CREATE))
		ft_destroy(data);
	while (i < data->num_philos)
	{
		if (thread_funtions(&data->philos[i].thread,
				dinner_simulation, &data->philos[i], CREATE))
			ft_destroy(data);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		if (thread_funtions(&data->philos[i].thread, NULL, NULL, JOIN) != 0)
			ft_destroy(data);
		i++;
	}
	if (thread_funtions(&observer, NULL, NULL, JOIN) != 0)
		ft_destroy(data);
}
