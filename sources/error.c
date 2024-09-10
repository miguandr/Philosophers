/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:21:21 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/07 14:36:10 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_error(int error_num)
{
	const char	*message;
	const char	*error_message[] = {
		"invalid amount of arguments\n", //0
		"invalid character found\n", //1
		"invalid number of philisophers\n", //2
		"invalid time to die\n", //3
		"invalid time to eat\n", //4
		"invalid time to sleep\n", //5
		"invalid number of times each philosopher must eat\n", //6
		"error getting the time\n", //7
	};

	message = error_message[error_num];
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("Correct input: ", 1);
	ft_putstr_fd("./philo 5 800 200 200 [7]\n", 1);
	return (1);
}

int	ft_error2(int error_num)
{
	const char	*message;
	const char	*error_message[] = {
		"invalid type for mutex handle\n", //0
		"The value specified by mutex is invalid\n", //1
		"The value specified by attr is invalid\n", //2
		"A deadlock would occur if the thread blocked waiting for mutex\n", //3
		"The current thread does not hold a lock on mutex\n", //4
		"The process cannot allocate enough memory to create another mutex\n",
		"Mutex is locked\n", //6
		"Wrong type for thread handle\n", //7
		"No resources to create another thread\n", //8
		"The caller does not have appropiate permission\n", //9
		"The value specified by attr is invalid\n", //10
		"The value specified by thread is not joinable\n", //11
		"No thread could be found corresponding to that specified "
		"by the give thread ID\n", //12
		"A deadlock was detected or the value of thread specifies "
		"the calling thread\n", //13
	};

	message = error_message[error_num];
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 1);
	return (1);
}
