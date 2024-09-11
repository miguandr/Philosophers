/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 18:22:00 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/11 20:35:08 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
