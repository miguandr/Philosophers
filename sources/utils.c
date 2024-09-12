/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 18:04:19 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/12 14:37:05 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * Writes a string to a file descriptor.
 * @s: The string to write.
 * @fd: The file descriptor to write to.
 *
 * This function writes each character of the string one by one to the file
 * descriptor specified by fd.
 */
void	ft_putstr_fd(const char *s, int fd)
{
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

/**
 * Converts a string to a long integer.
 * @str: The string to convert.
 *
 * This function skips any leading whitespace, handles optional signs,
 * and converts the string to a long integer. It stops at the first
 * non-numeric character.
 */
long	ft_atol(const char *str)
{
	long	result;
	int		i;
	int		sign;

	i = 0;
	result = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == 32))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}
	return (result * sign);
}
