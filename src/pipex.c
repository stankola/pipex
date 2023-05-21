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
#include <sys/wait.h>
#include "libft.h"

void	append_string_array(char **arr[], char *str)
{
	char	**new_arr;
	int		size;
	int		i;

	size = 0;
	while ((*arr)[size] != NULL)
		size++;
	new_arr = malloc(sizeof(char **) * size + 2);
	if (new_arr == NULL)
		return ;
	i = 0;
	while (i < size)
	{
		new_arr[i] = (*arr)[i];
		ft_printf("%s\n", new_arr[i]);
		i++;
	}
	new_arr[i++] = str;
	new_arr[i] = NULL;
	free(*arr);
	*arr = new_arr;
}

char	*run_cmd(char *cmd, char *input[], char *filename)
{
	int	pid;
	int pid2;

	append_string_array(&input, filename);
	pid = fork();
	ft_printf("pid %d\n", pid);
	pid2 = wait(NULL);
	ft_printf("pid2 %d\n", pid2);
	execve(cmd, input, NULL);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	char	**arg1;

	if (argc != 5)
		return (1);	// Some error message here
	arg1 = ft_split(argv[2], ' ');
	for (int i = 0; i < 3; i++)
		ft_printf("%s\n", arg1[i]);
	if (access(argv[1], R_OK))
	{
		perror(argv[1]);
		return (1);
	}
/*	if (! access(argv[2], X_OK))
	{
		ft_printf("file is ok to be run\n");
	}
	else
	{
		perror(argv[2]);
		return (1);
	}*/
	// read file argv[1]
	run_cmd(arg1[0], arg1, argv[1]);
	// send input to argv[2]
	// redirect argv[2] output to argv[3]
	// write output to file argv[4]
	return (1);
}