#include "philo.h"

static char	*message(int mess)
{
	if (mess == 0)
		return (" is eating\n");
	else if (mess == 1)
		return (" is sleeping\n");
	else if (mess == 2)
		return (" has taken a fork\n");
	else if (mess == 3)
		return (" is thinking\n");
	else if (mess == 5)
		return ("must eat count reached\n");
	return (" died\n");
}

void	print_message(t_philo *philo, int mess)
{
	static int	flag = 0;

	pthread_mutex_lock(&philo->param->writing);
	if (!flag)
	{
		printf("time:%u  ", get_time() - philo->param->start_time);
		if (mess != 5)
			printf("%d", philo->id + 1);
		if (mess >= 4)
			flag = 1;
		printf("%s", message(mess));
	}
	pthread_mutex_unlock(&philo->param->writing);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->param->forks[philo->left_fork]);
	print_message(philo, 2);
	pthread_mutex_lock(&philo->param->forks[philo->right_fork]);
	print_message(philo, 2);
}

void	sleep_philo(t_philo *philo)
{
	print_message(philo, 1);
	pthread_mutex_unlock(&philo->param->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->param->forks[philo->right_fork]);
	usleep(philo->param->time_to_sleep * 1000);
}

void	eat_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->to_do);
	philo->eat_now = 1;
	philo->start_time_eat = get_time();
	philo->limit_time = philo->start_time_eat + philo->param->time_to_die;
	print_message(philo, 0);
	usleep(philo->param->time_to_eat * 1000);
	philo->eat_count++;
	philo->eat_now = 0;
	pthread_mutex_unlock(&philo->to_do);
	pthread_mutex_unlock(&philo->eating);
}
