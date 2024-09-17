/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 23:20:55 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/17 12:41:27 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	end_simulation(t_data *data)
{
	bool	ended;

	mutex_functions(&data->mutex, LOCK);
	ended = data->end_simulation;
	mutex_functions(&data->mutex, UNLOCK);
	return (ended);
}

/**
 * Checks if all philosophers have eaten the required number of times.
 * @data: Pointer to the data structure holding simulation information.
 *
 * This function iterates through the philosophers and checks if each has eaten
 * the required number of meals. If all philosophers have eaten, it returns true
 * and prints a message. Otherwise, it returns false.
 */
static int	philos_full(t_data *data)
{
	int	i;
	int	meals_eaten;

	i = 0;
	meals_eaten = 0;
	if (data->num_times_to_eat == -1)
		return (0);
	while (i < data->num_philos)
	{
		mutex_functions(&data->philos[i].philo_mtx, LOCK);
		if (data->philos[i].meals_eaten >= data->num_times_to_eat)
			meals_eaten++;
		mutex_functions(&data->philos[i].philo_mtx, UNLOCK);
		i++;
	}
	if (meals_eaten == data->num_philos)
	{
		mutex_functions(&data->dead_lock, LOCK);
		data->dead_flag = 1;
		mutex_functions(&data->dead_lock, UNLOCK);
		mutex_functions(&data->print_lock, LOCK);
		printf("\nAll philosophers have eaten! 🍝\n");
		mutex_functions(&data->print_lock, UNLOCK);
		return (1);
	}
	return (0);
}

/**
 * Checks if a philosopher has died based on the time of their last meal.
 * @data: Pointer to the data structure holding simulation information.
 * @philo: Pointer to the philosopher to check.
 *
 * This function locks the philosopher's mutex to retrieve the time of their
 * last meal, then unlocks it. If the time since their last meal exceeds
 * the time to die, the philosopher is declared dead and the simulation ends.
 */
static int	philos_dead(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		mutex_functions(&philo[i].philo_mtx, LOCK);
		if ((get_time() - philo[i].last_meal) >= data->time_to_die
			&& philo[i].eating == 0)
		{
			print_status(philo[i].id, "died", data);
			mutex_functions(&data->dead_lock, LOCK);
			data->dead_flag = 1;
			mutex_functions(&data->dead_lock, UNLOCK);
			mutex_functions(&philo[i].philo_mtx, UNLOCK);
			return (1);
		}
		mutex_functions(&philo[i].philo_mtx, UNLOCK);
		i++;
	}
	return (0);
}

/**
 * The observer thread that monitors the philosophers for death or completion.
 * @pointer: A pointer to the data structure passed to the thread.
 *
 * This function runs in a loop, checking each philosopher for death or if all
 * philosophers have eaten the required number of meals. The loop terminates
 * when the simulation ends. It periodically sleeps for 10 milliseconds
 * between checks.
 */
void	*ft_observer(void *pointer)
{
	t_data	*data;
	t_philo	*philo;

	data = (t_data *)pointer;
	philo = data->philos;
	while (1)
	{
		if (philos_dead(data, philo) == 1)
			break ;
		if (philos_full(data) == 1)
			break ;
		//ft_usleep(10);
	}
	return (pointer);
}
