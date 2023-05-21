/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 18:23:58 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/20 18:23:59 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

char	*run_cmd(char *cmd, char *input[])
{
	int	pid;
	ft_printf("%s\n", input[0]);
	pid = fork();
	ft_printf("%d\n", pid);
	//execve(cmd, input, NULL);
	return (NULL);
}

char	*split_until(char *c, char delim, char limit)
{
	
}

int	main(int argc, char *argv[])
{
	// Plan:
	// 1. Parse input
	// get cmd1
	// get list of arguments for cmd1
	// 

	char	**arg1;

	if (argc != 5)
		return (1);	// Some error message here
	if (! access(argv[1], R_OK))
	{
		arg1 = ft_split(argv[1], ' ');
		ft_printf("file is ok to be read\n");
	}
	else
	{
		perror(argv[1]);
		return (1);
//		ft_printf("%s\n", strerror);	// file is not ok to read
	}
	if (! access(argv[2], X_OK))
	{
		ft_printf("file is ok to be run\n");
	}
	else
	{
		perror(argv[2]);
		return (1);
	}
	// read file argv[1]
	run_cmd(argv[2], &argv[3]);
	// send input to argv[2]
	// redirect argv[2] output to argv[3]
	// write output to file argv[4]
	return (1);
}