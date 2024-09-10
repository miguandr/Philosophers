/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:49:08 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/10 18:06:47 by miguandr         ###   ########.fr       */
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

/*ANSI Colors*/
# define RST "\033[0m"
# define R "\033[1m\033[31m"
# define G "\033[1m\033[32m"
# define Y "\033[1m\033[33m"
# define B "\033[1m\033[34m"
# define W "\033[1m\033[37m"

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

// Is it really needed? not confuse with t_fork
typedef struct s_forks
{
	t_mtx	fork;
	int		id;
}	t_forks;

struct					s_philo;
typedef struct s_philo	t_philo;

typedef struct s_data
{
	t_philo		*philos;
	t_mtx		mutex; // mutex for data struct access
	int			num_philos;
	int			ready_count;
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	int			num_times_to_eat;
	bool		dead_flag; // No se ha usado
	size_t		start_simmulation;
	bool		end_simulation;
	t_mtx		print_lock; // only for printing status
	t_forks		*forks;
}	t_data;

typedef struct s_philo
{
	t_data		*data;
	pthread_t	thread;
	int			id;
	int			meals_eaten;
	size_t		last_meal;
	bool		full;
	t_mtx		*right_fork;
	t_mtx		*left_fork;
	t_mtx		philo_mtx; // new
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

/*****-Initialization-*****/
void	init_data(t_data *data, t_forks *forks, t_philo *philos, char **value);

/*****-Simulation-*****/
void		start_program(t_data *data);
//void	start_philos(t_data *data);
//void	*dinner_simulation(void *pointer);

/*****-Simulation Utils-*****/
void	set_ready_count(t_data *data);
bool	get_philos_ready(t_data *data);
size_t	get_time(void);
void	print_status(int id, char *str, t_data *data);
int		ft_destroy(t_data *data);

/*****-Utils-*****/
void	ft_putstr_fd(const char *s, int fd);
long	ft_atol(const char *nptr);
void	ft_putnbr_fd(int n, int fd); // BORRAR

/*****-Observer-*****/
void	*ft_observer(void *pointer);

#endif
