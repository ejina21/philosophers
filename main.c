#include "philo.h"

static void	full_clean(t_param *param)
{
	int	i;

	i = 0;
	if (param->philo)
	{
		while (i < param->count_philo)
		{
			pthread_mutex_destroy(&param->philo[i].to_do);
			pthread_mutex_destroy(&param->philo[i++].eating);
		}
		free(param->philo);
		pthread_mutex_destroy(&param->writing);
		pthread_mutex_destroy(&param->check_dead);
		i = 0;
		if (param->forks)
		{
			while (i < param->count_philo)
				pthread_mutex_destroy(&param->forks[i++]);
			free(param->forks);
		}
	}
}

int	main(int argc, char **argv)
{
	t_param	param;

	if (argc < 5 || argc > 6)
		return (printf("ERROR\n"));
	if (pull_param(argc, argv, &param))
	{
		full_clean(&param);
		return (printf("ERROR\n"));
	}
	if (create_pthread(&param))
	{
		full_clean(&param);
		return (printf("ERROR\n"));
	}
	pthread_mutex_lock(&param.check_dead);
	pthread_mutex_unlock(&param.check_dead);
	full_clean(&param);
	return (0);
}
