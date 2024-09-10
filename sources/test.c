#include "../includes/philo.h"

void	ft_putstr_fd(const char *s, int fd)
{
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

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

static void	assign_forks(t_philo *philos, t_forks *forks, int num_philos)
{
	int	i;
	int	left_i;
	int	right_i;

	i = 0;
	while (i < num_philos)
	{
		right_i = i;
		left_i = (i + 1) % num_philos;
		if (right_i < left_i)
		{
			philos[i].right_fork = &forks[right_i].fork;
			philos[i].left_fork = &forks[left_i].fork;
		}
		else
		{
			philos[i].right_fork = &forks[left_i].fork;
			philos[i].left_fork = &forks[right_i].fork;
		}
		i++;
	}
}

static void	init_philos(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].full = false;
		philos[i].last_meal = get_time();
		i++;
	}
	assign_forks(philos, data->forks, data->num_philos);
}

static void	init_input(t_data *data, char **value)
{
	data->num_philos = ft_atol(value[1]);
	data->time_to_die = ft_atol(value[2]) * 1000; // * 1000 conversion to miliseconds (usleep)
	data->time_to_eat = ft_atol(value[3]) * 1000;
	data->time_to_sleep = ft_atol(value[4]) * 1000;
	if (value[5])
		data->num_times_to_eat = ft_atol(value[5]);
	else
		data->num_times_to_eat = -1;
}

void	init_data(t_data *data, t_forks *forks, t_philo *philos, char **value)
{
	int	i;

	data->philos = philos;
	data->forks = forks;
	data->dead_flag = 0;
	data->end_simulation = false;
	data->ready_count = 0;
	init_input(data, value);
	if ((mutex_functions(&data->mutex, INIT)
			|| mutex_functions(&data->print_lock, INIT)) != 0)
		return ;
	i = 0;
	while (i < data->num_philos)
	{
		if (mutex_functions(&forks[i].fork, INIT) != 0)
			return ;
		forks[i].id = i; // needed maybe for debbuging?
		i++;
	}
	init_philos(philos, data);
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
	ft_putstr_fd(R"ERROR: "RST, 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd(B"Correct input: "RST, 1);
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
	ft_putstr_fd(R"ERROR: "RST, 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 1);
	return (1);
}

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
	return (0);
}

int	mutex_functions(t_mtx *mutex, t_mtx_type type)
{
	int	status;

	if (type == LOCK)
		status = pthread_mutex_lock(mutex);
	if (type == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (type == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (type == DESTROY)
		status = pthread_mutex_destroy(mutex);
	else
		return (ft_error2(0));
	return (mutex_error(status, type));
}

int	thread_error(int status, t_mtx_type type)
{
	if (status == EAGAIN)
		return (ft_error2(8));
	else if (status == EPERM)
		return (ft_error2(9));
	if (status == EINVAL && type == CREATE)
		return (ft_error2(10));
	else if (status == EINVAL && (type == INIT || type == DETACH))
		return (ft_error2(11));
	else if (status == ESRCH)
		return (ft_error2(12));
	else if (status == EDEADLK)
		return (ft_error2(13));
	return (0);
}

int	thread_funtions(pthread_t *thread, void *(*foo)(void *),
	void *data, t_mtx_type type)
{
	if (type == CREATE)
		return (thread_error(pthread_create(thread, NULL, foo, data), type));
	if (type == JOIN)
		return (thread_error(pthread_join(*thread, NULL), type));
	else if (type == DETACH)
		return (thread_error(pthread_detach(*thread), type));
	else
		return (ft_error2(7));
}

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_error(7);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_status(int id, char *str, t_data *data) // Add mutex if used by more threads
{
	size_t	time;

	mutex_functions(&data->print_lock, LOCK);
	time = get_time() - data->start_simmulation;
	if (!data->end_simulation)
		printf("%zu: philo %d %s\n", time, id, str);
	mutex_functions(&data->print_lock, UNLOCK);
}

bool	philos_full(t_data *data)
{
	int	i;

	i = 0;
	if (data->num_times_to_eat == -1)
		return (false);
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->num_times_to_eat)
			return (false);
		i++;
	}
	return (true);
}

void	philos_dead(t_data *data, t_philo *philo)
{
	if ((get_time() - philo->last_meal) > data->time_to_die)
	{
		data->end_simulation = true;
		print_status(philo->id, "died", data);
	}
}

void	*ft_observer(void *pointer)
{
	t_data	*data;
	t_philo	*philo;
	int		i;

	data = (t_data *)pointer;
	while (!data->end_simulation)
	{
		i = 0;
		while (i < data->num_philos)
		{
			philo = &data->philos[i];
			mutex_functions(&data->mutex, LOCK);
			philos_dead(data, philo);
			mutex_functions(&data->mutex, UNLOCK);
			if (data->end_simulation)
				break ;
			i++;
		}
		mutex_functions(&data->mutex, LOCK);
		if (philos_full(data))
			data->end_simulation = true;
		mutex_functions(&data->mutex, UNLOCK);
		usleep(100);
	}
	return (pointer);
}

void	set_ready_count(t_data *data)
{
	mutex_functions(&data->mutex, LOCK); // Lock the data's mutex to ensure exclusive access.
	data->ready_count++;
	mutex_functions(&data->mutex, UNLOCK);
}

bool	get_philos_ready(t_data *data)
{
	int	ready_count;

	mutex_functions(&data->mutex, LOCK);
	ready_count = data->ready_count;
	mutex_functions(&data->mutex, UNLOCK);
	return (ready_count == data->num_philos);
}

void	one_philo(t_data *data, t_philo *philo)
{
	mutex_functions(philo->right_fork, LOCK);
	print_status(philo->id, "has taken the right fork", data);
	usleep(data->time_to_die);
	mutex_functions(philo->right_fork, UNLOCK);
}

void	ft_eat(t_data *data, t_philo *philo)
{
	if (data->num_philos == 1)
	{
		one_philo(data, data->philos);
		return ;
	}
	mutex_functions(philo->right_fork, LOCK);
	print_status(philo->id, "took the right fork", data);
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

void	philos_routine(t_data *data, t_philo *philo) //TO DO
{
	while (!data->end_simulation)
	{
		ft_eat(data, philo);
		print_status(philo->id, "is sleeping", philo->data);
		usleep(philo->data->time_to_sleep);
		print_status(philo->id, "is thinking", philo->data);
	}
}

void	*dinner_simulation(void *pointer)
{
	t_philo	*philos;
	t_data	*data;

	philos = (t_philo *)pointer;
	data = philos->data;

	set_ready_count(data);
	while (!get_philos_ready(data))
		usleep(100);
	while (!data->end_simulation)
	{
		philos_routine(data, philos);
	}
	return (NULL);
}

int	ft_destroy(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	status = mutex_functions(&data->print_lock, DESTROY);
	if (status != 0)
		return (status);
	status = mutex_functions(&data->mutex, DESTROY);
	if (status != 0)
		return (status);
	while (i < data->num_philos)
	{
		status = mutex_functions(&data->forks->fork, DESTROY);
		if (status != 0)
			return (status);
		i++;
	}
	return (0);
}

void	start_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (thread_funtions(&data->philos[i].thread, dinner_simulation,
				&data->philos[i], CREATE) != 0)
		{
			ft_destroy(data); //TO DO
			return ;
		}
		i++;
	}
	while (!get_philos_ready(data))
		usleep(100);
}

int	start_program(t_data *data)
{
	pthread_t	observer;
	int			i;

	i = 0;
	if (data->num_times_to_eat == 0)
		return (ft_destroy(data)); // back to main and clean (necessary if no malloc?)
	start_philos(data);
	if (thread_funtions(&observer, ft_observer, &data->philos, CREATE) != 0)
		return (ft_destroy(data)); //TO DO
	data->start_simmulation = get_time();
	while (i < data->num_philos)
	{
		if (thread_funtions(&data->philos[i].thread, NULL,
				NULL, JOIN) != 0)
			return (ft_destroy(data)); //TO DO
		i++;
	}
	if (thread_funtions(&observer, NULL, NULL, JOIN) != 0)
		return (ft_destroy(data)); //TO DO
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	philos[MAX_PHILOS];
	t_forks	forks[MAX_PHILOS];

	if (ac != 5 && ac != 6)
		return (ft_error(0));
	printf("entro");
	if (check_input(ac, av))
		return (1);
	init_data(&data, forks, philos, av);
	start_program(&data);
	return (0);
}
