/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:02:27 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/10 18:13:23 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	ft_eat(t_data *data, t_philo *philo)
{
	mutex_functions(philo->right_fork, LOCK);
	print_status(philo->id, "took the right fork", data);
	if (data->num_philos == 1)
	{
		usleep(data->time_to_die * 1000);
		mutex_functions(philo->right_fork, UNLOCK);
		return ;
	}
	mutex_functions(philo->left_fork, LOCK);
	print_status(philo->id, "took the left fork", data);
	// do i need a philo mutex?
	philo->meals_eaten++;
	print_status(philo->id, "is eating", data);
	usleep(data->time_to_eat);
	philo->last_meal = get_time(); // check if placing here or before usleep
	if (philo->meals_eaten == data->num_times_to_eat)
		philo->full = true;
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
		usleep(philo->data->time_to_sleep);
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
		usleep(100);
	data->start_simmulation = get_time();
	while (!data->end_simulation)
	{
		philos_routine(data, philos);
	}
	return (NULL);
}

static void	start_philos(t_data *data)
{
	int			i;
	int			status;

	i = 0;
	while (i < data->num_philos)
	{
		status = pthread_create(&data->philos[i].thread, NULL, dinner_simulation, &data->philos[i]);
		if (status != 0)
		{
			ft_putstr_fd("problema en pthread_create\n", 1);
			ft_destroy(data); // Ensure proper cleanup
			return;
		}
		i++;
	}
	//ft_putstr_fd("los philos estan listos\n", 1);
	while (!get_philos_ready(data))
		usleep(100);
}

void	start_program(t_data *data)
{
	pthread_t	observer;
	int			i;

	i = 0;
	if (data->num_times_to_eat == 0)
		return ;
	//if (thread_funtions(&observer, ft_observer, &data->philos, CREATE) != 0)
	if (pthread_create(&observer, NULL, ft_observer, &data->philos))
	{
		ft_destroy(data);
		return ;
	}
	start_philos(data);
	//data->start_simmulation = get_time();
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
