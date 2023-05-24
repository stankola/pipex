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
#include <string.h>
#include "libft.h"
#include "pipex.h"
#ifndef PIPEX_READ_BUFFER
# define PIPEX_READ_BUFFER 1
#endif

void	pipe_runner(char **cmds, int input, int output)
{
	int		pid;
	char	*cmd;

//	ft_fprintf(STDERR_FILENO, "Child here, input %d output %d cmd %s\n", input, output, cmds[0]);
	cmd = find_cmd(cmds[0]);
	if (cmd == NULL)
	{
		perror(cmds[0]);
		exit(-1);
	}
	dup2(input, STDIN_FILENO);
	dup2(output, STDOUT_FILENO);
	pid = fork();
	if (pid > 0)
	{
		wait(NULL);
		free(cmd);
		exit (0);
	}
	else if (pid == 0)
	{
		if (access(cmd, X_OK) == 0)
			execve(cmd, cmds, __environ);	// Check the environ variable name
		perror(cmd);
		exit(-1);
	}
	perror("Child fork fail");
	free(cmd);
	exit(-1);
}

// returns fd for output
int	pipe_starter(int input_fd, int output_fd, char ***cmds)
{
	int	pipe_fds[2];
	int	pid;

	while (*(cmds) != NULL)
	{
		if (pipe(pipe_fds) < 0)
		{
			perror("The pipes break!");
			return (-1);
		}
		pid = fork();
		if (pid > 0)
		{
			close(pipe_fds[CHILD_END]);								// Could fail
//			ft_printf("The wait is over, woken by %d\n", waitpid(pid, NULL, 0));
			input_fd = pipe_fds[PARENT_END];
//			ft_fprintf(STDERR_FILENO, "Parent here, fd %d fd %d\n", pipe_fds[PARENT_END], input_fd);
		}
		else if (pid == 0)
		{
//			ft_printf("Child is %d\n", getpid());
			close(pipe_fds[PARENT_END]);							// Could fail
			if (*(cmds + 1) == NULL)
				pipe_runner(*cmds, input_fd, output_fd);
			else
				pipe_runner(*cmds, input_fd, pipe_fds[CHILD_END]);
		}
		else
		{
			perror("The forks break!");
			return (-1);
		}
		cmds++;
	}
	waitpid(pid, NULL, 0);		// Wait until the last child process is done before returning and closing parent end of the pipes(?)
	return (input_fd);		// This return value seems unnecessary
}

void	read_stdin(char* limiter, int output)
{
	char	*limit_check;
	char	*readbuf;
	long int		i;

	readbuf = malloc(sizeof(PIPEX_READ_BUFFER));
	while (1)
	{
		limit_check = limiter;
		read(STDIN_FILENO, readbuf, PIPEX_READ_BUFFER);
		while (*limit_check != '\0' && *readbuf == *(limit_check))
		{
			*limit_check = *readbuf;
			read(STDIN_FILENO, readbuf, PIPEX_READ_BUFFER);
			limit_check++;
		}
		if (*limit_check == '\0')
			break ;
		i = 0;
		while (limit_check != &(limiter[i]))
			write(output, &(limiter[i++]), 1);
		write(output, readbuf, 1);
	}
	free(readbuf);
}

void	pipe_limiter(char *limiter, int output, char ***cmds)
{
	int		pid;
	int		pipe_fds[2];
	int		stdin;

	if (pipe(pipe_fds) < 0)
	{
		perror("Pipes break!");
		return ;
	}
	pid = fork();
	if (pid > 0)
	{
		close(pipe_fds[CHILD_END]);
		pipe_starter(pipe_fds[PARENT_END], output, cmds);
		waitpid(pid, NULL, 0);
	}
	else if (pid == 0)
	{
		close(pipe_fds[PARENT_END]);
		read_stdin(limiter, pipe_fds[CHILD_END]);
		exit(0);
	}
	else
		ft_fprintf(STDERR_FILENO, "FORKING FAILURES!");
}

char	***get_cmds(char *argv[], int argc)
{
	int		i;
	char	***cmds;
	
	i = 0;
	cmds = ft_calloc(sizeof(char **), (argc + 1));
	while (i < argc)
	{
		cmds[i] = ft_split(argv[i], ' ');
		i++;
	}
	return (cmds);
}

int	main(int argc, char *argv[])
{
	char	***cmds;
	int		fds[2];

	if (argc < 5)
		return (1);	// Some error message here ??
	if (ft_strncmp("here_doc", argv[1], 8) != 0)
	{
		fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP);	// This seems to be ok. bash also seems to create the target file beforehand
		if (fds[1] < 0)
			perror(argv[argc - 1]);
		fds[0] = open(argv[1], O_RDONLY);
		if (fds[0] < 0)
			perror(argv[1]);
		cmds = get_cmds(&argv[2], argc - 3);
		pipe_starter(fds[0], fds[1], cmds);
		free_strarrayarray(&cmds);
		return (0);
	}
	fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP);	// This seems to be ok. bash also seems to create the target file beforehand
	if (fds[1] < 0)
		perror(argv[argc - 1]);
	cmds = get_cmds(&argv[3], argc - 4);
	pipe_limiter(argv[2], fds[1], cmds);
	free_strarrayarray(&cmds);
	return (0);
}
