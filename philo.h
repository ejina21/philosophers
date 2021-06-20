#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

struct	s_param;

typedef struct s_philo
{
	int				id;
	int				eat_now;
	unsigned int	limit_time;
	unsigned int	start_time_eat;
	int				left_fork;
	int				right_fork;
	int				eat_count;
	struct s_param	*param;
	pthread_mutex_t	to_do;
	pthread_mutex_t	eating;
}					t_philo;

typedef struct s_param
{
	int				count_philo;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				count_eat_all;
	unsigned int	start_time;
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing;
	pthread_mutex_t	check_dead;
}					t_param;
int				pull_param(int argc, char **argv, t_param *param);
int				create_pthread(t_param *param);
unsigned int	get_time(void);
void			print_message(t_philo *philo, int mess);
void			take_forks(t_philo *philo);
void			sleep_philo(t_philo *philo);
void			eat_philo(t_philo *philo);
#endif