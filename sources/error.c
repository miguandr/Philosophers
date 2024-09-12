/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:21:21 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/12 14:22:16 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * Prints an error message based on the error code provided.
 * @error_num: Error code corresponding to a specific error message.
 *
 * This function prints out an error message related to invalid input
 * or failed operations. It also provides a hint about the correct input format.
 * Returns 1 after printing the message.
 */
int	ft_error(int error_num)
{
	const char	*message;
	const char	*error_message[] = {
		"invalid amount of arguments\n",
		"invalid character found\n",
		"invalid number of philisophers\n",
		"invalid time to die\n",
		"invalid time to eat\n",
		"invalid time to sleep\n",
		"invalid number of times each philosopher must eat\n",
		"error getting the time\n"
	};

	message = error_message[error_num];
	ft_putstr_fd("\n🚨 ERROR: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("✅ Correct input: ", 1);
	ft_putstr_fd("./philo 5 800 200 200 [7]\n", 1);
	return (1);
}

/**
 * Prints a detailed error message related to mutexes or threads.
 * @error_num: Error code corresponding to a specific error condition.
 *
 * This function provides detailed error messages for various issues
 * related to mutex handling or thread creation. It returns 1 after
 * printing the corresponding error message.
 */
int	ft_error2(int error_num)
{
	const char	*message;
	const char	*error_message[] = {
		"invalid type for mutex handle\n",
		"The value specified by mutex is invalid\n",
		"The value specified by attr is invalid\n",
		"A deadlock would occur if the thread blocked waiting for mutex\n",
		"The current thread does not hold a lock on mutex\n",
		"The process cannot allocate enough memory to create another mutex\n",
		"Mutex is locked\n",
		"Wrong type for thread handle\n",
		"No resources to create another thread\n",
		"The caller does not have appropiate permission\n",
		"The value specified by attr is invalid\n",
		"The value specified by thread is not joinable\n",
		"No thread could be found corresponding to that specified "
		"by the give thread ID\n",
		"A deadlock was detected or the value of thread specifies "
		"the calling thread\n"
	};

	message = error_message[error_num];
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 1);
	return (1);
}
