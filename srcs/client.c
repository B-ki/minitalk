/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorel <rmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:16:56 by rmorel            #+#    #+#             */
/*   Updated: 2022/03/28 09:44:48 by rmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdio.h>

t_mess	g_m = {NULL, NULL};

void	error(void)
{
	ft_printf("Erreur, 2 arguments only: ./client <pid> <str>\n");
	exit(0);
}

int	main(int ac, char **av)
{
	pid_t				pid;
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_handler = &handler;
	if (ac != 3 || av[1][0] == '\0' || av[2][0] == '\0')
		error();
	pid = (pid_t)ft_atoi(av[1]);
	if (pid <= 0 || pid > 2147483647)
		error();
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	g_m.size = int_to_bin(ft_strlen(av[2]));
	g_m.mess = binary_converter(av[2]);
	send_message(g_m.size, pid, 0);
	send_message(g_m.mess, pid, 1);
	free(g_m.size);
	free(g_m.mess);
	return (0);
}

char	*int_to_bin(int n)
{
	char	*bin;
	int		j;

	j = 31;
	bin = malloc(sizeof(char) * (32 + 1));
	if (!bin)
		return (NULL);
	while (j >= 0)
	{
		if (n & (1 << (31 - j)))
			bin[j] = 1 + '0';
		else
			bin[j] = 0 + '0';
		j--;
	}
	bin[32] = '\0';
	return (bin);
}

char	*binary_converter(char *str)
{
	char	*bin;
	int		i;
	int		j;
	char	c;

	i = -1;
	j = 8;
	c = str[0];
	bin = malloc(sizeof(char) * (ft_strlen(str) * 8 + 1));
	if (!bin)
		return (NULL);
	while (str[++i])
	{
		while (--j >= 0)
		{
			if (c & (1 << j))
				bin[i * 8 + 7 - j] = 1 + 48;
			else
				bin[i * 8 + 7 - j] = 0 + 48;
		}
		c = str[i + 1];
		j = 8;
	}
	bin[i * 8] = '\0';
	return (bin);
}

void	handler(int signum)
{
	static int	bits_received = 0;

	if (signum == SIGUSR1)
		ft_printf("All data have been printed !\n");
	if (signum == SIGUSR2)
		bits_received++;
}
