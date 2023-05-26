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
#include <sys/stat.h>
#include "libft.h"
#include "pipex.h"
#ifndef PIPEX_READ_BUFFER_SIZE
# define PIPEX_READ_BUFFER_SIZE 1
#endif

void	read_stdin(char* limiter, int output)
{
	char	*limit_check;
	char	*readbuf;
	long int		i;

	readbuf = malloc(sizeof(PIPEX_READ_BUFFER_SIZE));
	while (1)
	{
		limit_check = limiter;
		read(STDIN_FILENO, readbuf, PIPEX_READ_BUFFER_SIZE);
		while (*limit_check != '\0' && *readbuf == *(limit_check))
		{
			*limit_check = *readbuf;
			read(STDIN_FILENO, readbuf, PIPEX_READ_BUFFER_SIZE);
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
	exit(0);
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

int	forking_pipe(char **cmds, int pipe_write, int pipe_read, int prev_pipe, char *files[],
	int pipe_case, char *limit)
{
	int	pid;
	
	pid = fork();
	if (pid > 0)
	{
		close(pipe_write);
		return (pid);
	}
	else if (pid == 0)
	{
		close(pipe_read);
//		if (pipe_case == ppx_file_input)
//			pipe_file_input(cmds, files[PIPEX_IN], pipe_write);
		if (pipe_case == ppx_here_input)
			read_stdin(limit, pipe_write);
		else if (pipe_case == ppx_midpoint || pipe_case == ppx_file_input)
			pipe_command(cmds, prev_pipe, pipe_write);
		else if (pipe_case == ppx_out_append)
			pipe_file_output_append(cmds, prev_pipe, files[PIPEX_OUT]);
		else if (pipe_case == ppx_out_trunc)
			pipe_file_output_trunc(cmds, prev_pipe, files[PIPEX_OUT]);
	}
	else
		perror(NULL);
	return (-1);
}

int	get_input_fd(char *filename)
{
	int		input_fd;

	input_fd = open(filename, O_RDONLY);
	if (input_fd < 0)
		perror(filename);
	return (input_fd);
}

int	pipe_master(char ***cmds, char *files[], char *limit)
{
	int	fds[2];
	int	i;
	int	last_pid;
	int	prev_output;
	int	input_fd;

	i = -1;
	if (!limit)
		input_fd = get_input_fd(files[PIPEX_IN]);
	else
		input_fd = STDIN_FILENO;
	while (cmds[++i] != NULL)
	{
		if (pipe(fds) < 0)
			perror(NULL);
		if (limit && i == 0)
			last_pid = forking_pipe(cmds[i], fds[PIPE_WRITE], fds[PIPE_READ], prev_output, files, ppx_here_input, limit);
		else if (i == 0)
			last_pid = forking_pipe(cmds[i], fds[PIPE_WRITE], fds[PIPE_READ], input_fd, files, ppx_file_input, NULL);
		else if (limit && cmds[i + 1] == NULL)
			last_pid = forking_pipe(cmds[i], fds[PIPE_WRITE], fds[PIPE_READ], prev_output, files, ppx_out_append, limit);
		else if (cmds[i + 1] == NULL)
			last_pid = forking_pipe(cmds[i], fds[PIPE_WRITE], fds[PIPE_READ], prev_output, files, ppx_out_trunc, NULL);
		else
			last_pid = forking_pipe(cmds[i], fds[PIPE_WRITE], fds[PIPE_READ], prev_output, files, ppx_midpoint, NULL);
		prev_output = fds[PIPE_READ];
	}
	waitpid(last_pid, &i, 0);
	close(input_fd);
	return (i);
}

int	main(int argc, char *argv[])
{
	char	***cmds;
	char	*io_files[2];
//	int		result;

	if (argc < 5)
	{
		ft_fprintf(STDERR_FILENO, "Wrong number of arguments\n");
		return (-1);
	}
	io_files[PIPEX_OUT] = argv[argc - 1];
	if (ft_strncmp("here_doc", argv[1], 8) != 0)
	{
		io_files[PIPEX_IN] = argv[1];
		cmds = get_cmds(&argv[2], argc - 3);
		pipe_master(cmds, io_files, NULL);
		free_strarrayarray(&cmds);
		return (0);
	}
	cmds = get_cmds(&argv[2], argc - 3);
	pipe_master(cmds, io_files, argv[2]);
	free_strarrayarray(&cmds);
	return (0);
}
