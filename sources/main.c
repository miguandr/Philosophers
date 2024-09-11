/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:52:00 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/11 20:42:44 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	philos[MAX_PHILOS];
	t_mtx	forks[MAX_PHILOS];

	if (ac != 5 && ac != 6)
		return (ft_error(0));
	if (check_input(ac, av))
		return (1);
	init_data(&data, forks, philos, av);
	start_program(&data);
	ft_destroy(&data);
	printf("\nThe simulation has ended\n");
	return (0);
}
