#include "philo.h"

unsigned int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static void	*check_dead(void *philo_st)
{
	t_philo		*philo;

	philo = philo_st;
	while (1)
	{
		pthread_mutex_lock(&philo->to_do);
		if (!philo->eat_now && get_time() > philo->limit_time)
		{
			print_message(philo, 4);
			pthread_mutex_unlock(&philo->to_do);
			pthread_mutex_unlock(&philo->param->check_dead);
			return (0);
		}
		pthread_mutex_unlock(&philo->to_do);
		usleep(1000);
	}
}

static void	*check_count_eat(void *param_st)
{
	t_param	*param;
	int		i;
	int		i_eat;

	param = param_st;
	i_eat = 0;
	while (i_eat < param->count_eat_all)
	{
		i = 0;
		while (i < param->count_philo)
			pthread_mutex_lock(&param->philo[i++].eating);
		i_eat++;
	}
	print_message(&param->philo[0], 5);
	pthread_mutex_unlock(&param->check_dead);
	return (0);
}

static void	*do_common(void *philo_st)
{
	t_philo		*philo;
	pthread_t	pt_id;

	philo = philo_st;
	philo->start_time_eat = get_time();
	philo->limit_time = philo->start_time_eat + philo->param->time_to_die;
	if (pthread_create(&pt_id, NULL, &check_dead, philo_st))
		return ((void *)1);
	pthread_detach(pt_id);
	while (1)
	{
		take_forks(philo);
		eat_philo(philo);
		sleep_philo(philo);
		print_message(philo, 3);
	}
}

int	create_pthread(t_param *param)
{
	int			i;
	pthread_t	pt_id;
	t_philo		*philo;

	param->start_time = get_time();
	if (param->count_eat_all > 0)
	{
		if (pthread_create(&pt_id, NULL, &check_count_eat, param))
			return (1);
		pthread_detach(pt_id);
	}
	i = 0;
	while (i < param->count_philo)
	{
		philo = &param->philo[i];
		if (pthread_create(&pt_id, NULL, &do_common, philo))
			return (1);
		pthread_detach(pt_id);
		usleep(100);
		i++;
	}
	return (0);
}
