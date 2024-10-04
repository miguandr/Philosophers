/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:49:08 by miguandr          #+#    #+#             */
/*   Updated: 2024/10/04 17:12:47 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*Libraries*/
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/time.h>
# include <limits.h>
# include <errno.h>

/*Limits*/
# define MAX_PHILOS 200

/*Enums*/
typedef enum s_mtx_type
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}			t_mtx_type;

/*Structs*/
typedef pthread_mutex_t	t_mtx;

struct					s_philo;
typedef struct s_philo	t_philo;

typedef struct s_data
{
	t_philo		*philos;
	t_mtx		*forks;

	int			num_philos;
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	size_t		start_simulation;
	int			num_times_to_eat;
	int			dead_flag;
	int			full_flag;
	t_mtx		mutex;
	t_mtx		dead_lock;
	t_mtx		print_lock;
}	t_data;

typedef struct s_philo
{
	t_data		*data;
	pthread_t	thread;
	int			id;
	int			meals_eaten;
	int			eating;
	bool		has_eaten;
	size_t		last_meal;
	t_mtx		*right_fork;
	t_mtx		*left_fork;
	t_mtx		philo_mtx;

}	t_philo;

/*****-Checks-*****/
int		check_input(int ac, char **args);

/*****-Error_handling-*****/
int		ft_error(int error_num);
int		ft_error2(int error_num);

/*****-Mutex & Thread Handling-*****/
int		mutex_functions(t_mtx *mutex, t_mtx_type type);
int		mutex_error(int status, t_mtx_type type);
int		thread_funtions(pthread_t *thread, void *(*foo)(void *),
			void *data, t_mtx_type type);
int		thread_error(int status, t_mtx_type type);
void	ft_destroy(t_data *data);

/*****-Initialization-*****/
void	init_data(t_data *data, t_mtx *forks, t_philo *philos, char **value);

/*****-Simulation-*****/
void	start_program(t_data *data);

/*****-Simulation Utils-*****/
int		ft_usleep(size_t milliseconds);
size_t	get_time(void);
int		is_dead(t_data *data);
void	print_status(int id, char *str, t_data *data);

/*****-Observer-*****/
void	*ft_observer(void *pointer);

/*****-Utils-*****/
void	ft_putstr_fd(const char *s, int fd);
long	ft_atol(const char *nptr);

#endif
