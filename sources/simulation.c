/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:02:27 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/11 21:19:17 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	ft_eat(t_data *data, t_philo *philo)
{
	mutex_functions(philo->right_fork, LOCK);
	print_status(philo->id, "took the right fork", data);
	if (data->num_philos == 1)
	{
		ft_usleep(data->time_to_die);
		print_status(philo->id, "died 💀", data);
		data->end_simulation = true;
		mutex_functions(philo->right_fork, UNLOCK);
		return ;
	}
	mutex_functions(philo->left_fork, LOCK);
	print_status(philo->id, "took the left fork", data);
	mutex_functions(&philo->philo_mtx, LOCK);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	mutex_functions(&philo->philo_mtx, UNLOCK);
	print_status(philo->id, "is eating", data);
	ft_usleep(data->time_to_eat);
	mutex_functions(philo->right_fork, UNLOCK);
	mutex_functions(philo->left_fork, UNLOCK);
}

static void	philos_routine(t_data *data, t_philo *philo)
{
	while (!data->end_simulation)
	{
		ft_eat(data, philo);
		if (data->end_simulation)
			break ;
		print_status(philo->id, "is sleeping", philo->data);
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo->id, "is thinking", philo->data);
	}
}

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
	if (data->start_simmulation == 0)
		data->start_simmulation = get_time();
	mutex_functions(&data->mutex, UNLOCK);
	philos->last_meal = data->start_simmulation;
	if (philos->id % 2 == 0)
		ft_usleep(data->time_to_eat / 2);
	while (!data->end_simulation)
		philos_routine(data, philos);
	return (NULL);
}

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
			ft_putstr_fd("problema en pthread_create\n", 1);
			ft_destroy(data);
			return ;
		}
		i++;
	}
}

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
