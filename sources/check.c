/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 18:22:00 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/12 14:21:35 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * Checks if the provided arguments are valid integers.
 * @ac: Argument count.
 * @args: Array of arguments (strings).
 *
 * This function iterates through each argument and checks if
 * they contain only numeric characters (digits).
 * Returns 1 if a non-numeric character is found, otherwise returns 0.
 */
static int	check_args(int ac, char **args)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] < '0' || args[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

/**
 * Validates the input arguments for the philosophers simulation.
 * @ac: Argument count.
 * @args: Array of arguments (strings).
 *
 * This function verifies that the arguments are within acceptable ranges
 * for the number of philosophers and the timing parameters. If any
 * invalid input is detected, it returns an error code and message.
 */
int	check_input(int ac, char **args)
{
	if (check_args(ac, args))
		return (ft_error(1));
	if (ft_atol(args[1]) > MAX_PHILOS || ft_atol(args[1]) < 1)
		return (ft_error(2));
	if (ft_atol(args[2]) < 60 || ft_atol(args[2]) > INT_MAX)
		return (ft_error(3));
	if (ft_atol(args[3]) < 60 || ft_atol(args[3]) > INT_MAX)
		return (ft_error(4));
	if (ft_atol(args[4]) < 60 || ft_atol(args[4]) > INT_MAX)
		return (ft_error(5));
	if ((args[5] && ft_atol(args[4]) < 1) || (ft_atol(args[4]) > INT_MAX))
		return (ft_error(6));
	return (0);
}
