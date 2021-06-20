#include "philo.h"

static int	ft_atoi(char *str)
{
	int	sum;

	sum = 0;
	if (!*str)
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		sum = sum * 10 + *str - '0';
		str++;
	}
	if (*str != 0)
		return (-1);
	return (sum);
}

static void	initial_phil(t_param *param)
{
	int	i;

	i = 0;
	while (i < param->count_philo)
	{
		param->philo[i].eat_now = 0;
		param->philo[i].id = i;
		param->philo[i].left_fork = i;
		param->philo[i].right_fork = (i + 1) % param->count_philo;
		param->philo[i].eat_count = 0;
		param->philo[i].param = param;
		pthread_mutex_init(&param->philo[i].to_do, NULL);
		pthread_mutex_init(&param->philo[i].eating, NULL);
		pthread_mutex_lock(&param->philo[i].eating);
		i++;
	}
}

static int	initial_mutex(t_param *param)
{
	int	i;

	pthread_mutex_init(&param->writing, NULL);
	pthread_mutex_init(&param->check_dead, NULL);
	pthread_mutex_lock(&param->check_dead);
	param->forks = malloc(sizeof(pthread_mutex_t) * param->count_philo);
	if (!(param->forks))
		return (1);
	i = 0;
	while (i < param->count_philo)
		pthread_mutex_init(&param->forks[i++], NULL);
	return (0);
}

int	pull_param(int argc, char **argv, t_param *param)
{
	param->count_philo = ft_atoi(argv[1]);
	param->time_to_die = ft_atoi(argv[2]);
	param->time_to_eat = ft_atoi(argv[3]);
	param->time_to_sleep = ft_atoi(argv[4]);
	param->philo = NULL;
	param->forks = NULL;
	if (argc == 6)
	{
		param->count_eat_all = ft_atoi(argv[5]);
		if (param->count_eat_all < 1)
			return (1);
	}
	else
		param->count_eat_all = 0;
	if (param->count_philo < 1 || param->count_philo > 200
		|| param->time_to_die < 60
		|| param->time_to_eat < 60 || param->time_to_sleep < 60)
		return (1);
	param->philo = malloc(sizeof(t_philo) * param->count_philo);
	if (!(param->philo))
		return (1);
	initial_phil(param);
	return (initial_mutex(param));
}
