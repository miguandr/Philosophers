/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_mutex_thread.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 22:17:54 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/12 14:32:38 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * Handles errors related to mutex operations based on the error status and type.
 * @status: The error code returned by a mutex function.
 * @type: The type of mutex operation (LOCK, UNLOCK, INIT, DESTROY).
 *
 * This function checks the status of a mutex operation and calls
 * the appropriate error handler. Returns 1 if an error is found, otherwise 0.
 */
int	mutex_error(int status, t_mtx_type type)
{
	if (status == EINVAL && (type == LOCK || type == UNLOCK || type == DESTROY))
		return (ft_error2(1));
	else if (status == EINVAL && type == INIT)
		return (ft_error2(2));
	else if (status == EDEADLK)
		return (ft_error2(3));
	else if (status == EPERM)
		return (ft_error2(4));
	else if (status == ENOMEM)
		return (ft_error2(5));
	else if (status == EBUSY)
		return (ft_error2(6));
	else
		return (0);
}

/**
 * Performs various mutex-related operations based on the type provided.
 * @mutex: Pointer to the mutex object.
 * @type: The type of operation (LOCK, UNLOCK, INIT, DESTROY).
 *
 * This function performs the appropriate mutex operation and checks for errors.
 * Returns 0 if the operation is successful, otherwise returns the error code.
 */
int	mutex_functions(t_mtx *mutex, t_mtx_type type)
{
	int	status;

	if (type == LOCK)
		status = pthread_mutex_lock(mutex);
	else if (type == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (type == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (type == DESTROY)
		status = pthread_mutex_destroy(mutex);
	else
		return (ft_error2(0));
	return (mutex_error(status, type));
}

/**
 * Handles errors related to thread operations based on the error status
 * and type.
 * @status: The error code returned by a thread function.
 * @type: The type of thread operation (CREATE, JOIN, DETACH).
 *
 * This function checks the status of a thread operation and calls
 * the appropriate error handler. Returns 1 if an error is found, otherwise 0.
 */
int	thread_error(int status, t_mtx_type type)
{
	if (status == EAGAIN)
		return (ft_error2(8));
	else if (status == EPERM)
		return (ft_error2(9));
	else if (status == EINVAL && type == CREATE)
		return (ft_error2(10));
	else if (status == EINVAL && (type == INIT || type == DETACH))
		return (ft_error2(11));
	else if (status == ESRCH)
		return (ft_error2(12));
	else if (status == EDEADLK)
		return (ft_error2(13));
	else
		return (0);
}

/**
 * Manages thread creation, joining, or detaching based on the
 * provided type.
 * @thread: Pointer to the pthread_t thread object.
 * @foo: Pointer to the function the thread will execute.
 * @data: Pointer to the data passed to the thread.
 * @type: The type of thread operation (CREATE, JOIN, DETACH).
 *
 * This function handles thread operations such as creating, joining,
 * or detaching threads.
 * It returns 0 on success or the appropriate error code on failure.
 */
int	thread_funtions(pthread_t *thread, void *(*foo)(void *),
	void *data, t_mtx_type type)
{
	if (type == CREATE)
		return (thread_error(pthread_create(thread, NULL, foo, data), type));
	else if (type == JOIN)
		return (thread_error(pthread_join(*thread, NULL), type));
	else if (type == DETACH)
		return (thread_error(pthread_detach(*thread), type));
	else
		return (ft_error2(7));
}

/**
 * Destroys all mutexes used by the philosophers simulation.
 * @data: Pointer to the data structure holding the mutexes.
 *
 * This function iterates through the forks and other mutexes
 * used in the simulation, destroying each one. Returns 0 on success,
 * or an error code if any mutex fails to be destroyed.
 */
int	ft_destroy(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->num_philos)
	{
		status = mutex_functions(&data->forks[i], DESTROY);
		if (status != 0)
			return (status);
		i++;
	}
	status = mutex_functions(&data->print_lock, DESTROY);
	if (status != 0)
		return (status);
	status = mutex_functions(&data->mutex, DESTROY);
	if (status != 0)
		return (status);
	return (0);
}
