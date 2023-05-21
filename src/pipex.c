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
#include <fcntl.h>
#include "libft.h"
#ifndef CHILD_END
# define CHILD_END 1
#endif
#ifndef PARENT_END
# define PARENT_END 0
#endif

// Probably useless.
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

void	child(int fildes)
{
	int		writecount = 0;

	writecount = ft_fprintf(fildes, "I am child!\n");
	close(fildes);
	exit(EXIT_SUCCESS);
}

void	parent(int fildes)
{
	char	*buff;
	id_t	rdcount;
	pid_t	pid;

	buff = malloc (sizeof(char) * 80);
//	ft_fprintf(fildes,"I am not a child!\n");
	pid = wait(NULL);
	ft_printf("The wait is over, woken by %d\n", pid);
	rdcount = read(fildes, buff, 80);
	ft_printf("Read %d chars\n", rdcount);
	ft_printf("This is not a child, I hear %s\n", buff);
	free(buff);

}

char	*run_cmd(char *cmd, char *argv[], int fd)
{
	pid_t	pid;
	int		fildes[2];

	if (pipe(fildes) < 0)
	{
		perror("Piping failure");
		return (NULL);
	}
	pid = fork();
	if (pid == 0)
	{
		ft_printf("Child is %d fd %d\n", getpid(), fildes[CHILD_END]);
		close(fildes[PARENT_END]);
		if (dup2(fildes[CHILD_END], STDOUT_FILENO) < 0 || dup2(fd, STDIN_FILENO) < 0)
			return (NULL);	// How to properly handle this situation? Should fildes[CHILD_END] be closed?
		execve(cmd, argv, NULL);
	}
	else if (pid > 0)
	{
		close(fildes[CHILD_END]);
		parent(fildes[PARENT_END]);
	}
	else
		perror("Forking failure");
	return (NULL);
}

int	main(int argc, char *argv[])
{
	char	**arg1;
	char	**arg2;
	int		fd1;

	if (argc != 5)
		return (1);	// Some error message here
	arg1 = ft_split(argv[2], ' ');
	arg2 = ft_split(argv[3], ' ');
	if (access(argv[1], R_OK))	// This is ok. Bash checks for the file existence as well
	{
		perror(argv[1]);
		return (1);
	}
	fd1 = open(argv[1], O_RDONLY);
	if (access(argv[2], X_OK))
	{
		perror(argv[2]);
		return (1);
	}
	// read file argv[1]
	run_cmd(arg1[0], arg1, fd1);
	// send input to argv[2]
	// redirect argv[2] output to argv[3]
	// write output to file argv[4]
	return (0);
}