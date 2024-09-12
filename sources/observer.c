/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 23:20:55 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/12 14:52:51 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * Checks if all philosophers have eaten the required number of times.
 * @data: Pointer to the data structure holding simulation information.
 *
 * This function iterates through the philosophers and checks if each has eaten
 * the required number of meals. If all philosophers have eaten, it returns true
 * and prints a message. Otherwise, it returns false.
 */
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
	printf("\nAll philosophers have eaten! 🍝\n");
	return (true);
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
static void	philos_dead(t_data *data, t_philo *philo)
{
	size_t	last_meal;

	mutex_functions(&philo->philo_mtx, LOCK);
	last_meal = philo->last_meal;
	mutex_functions(&philo->philo_mtx, UNLOCK);
	if ((get_time() - last_meal) > data->time_to_die)
	{
		print_status(philo->id, "died", data);
		data->end_simulation = true;
	}
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
		ft_usleep(10);
	}
	return (pointer);
}
