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

int	forking_pipe(char **cmds, int fds[], char *files[],
	int pipe_case, char *limit)
{
	int	pid;
	int	local_fds[3];
	
	local_fds[PIPE_READ] = fds[PIPE_READ];
	local_fds[PIPE_WRITE] = fds[PIPE_WRITE];
	local_fds[2] = fds[2];
	pid = fork();
	if (pid == 0)
	{
		close(local_fds[PIPE_READ]);
		if (pipe_case == ppx_file_input)
			pipe_file_input(cmds, files[PIPEX_IN], local_fds[PIPE_WRITE]);
		if (pipe_case == ppx_here_input)
			read_stdin(limit, local_fds[PIPE_WRITE]);
		else if (pipe_case == ppx_midpoint)
			pipe_command(cmds, local_fds[2], local_fds[PIPE_WRITE]);
		else if (pipe_case == ppx_out_append)
			pipe_file_output_append(cmds, local_fds[2], files[PIPEX_OUT]);
		else if (pipe_case == ppx_out_trunc)
			pipe_file_output_trunc(cmds, local_fds[2], files[PIPEX_OUT]);
	}
	else if (pid < 0)
		perror(NULL);
	close(fds[PIPE_WRITE]);
	return (pid);
}

void	get_input_fd(char *filename, int *fd2, int *fd3, char *limit)
{
	if (limit)
	{
		*fd2 = 0;
		*fd3 = *fd2;
	}
	else
	{
		*fd3 = open(filename, O_RDONLY);
	//	if (*fd3 < 0)
	//		perror(filename);
		*fd2 = *fd3;
	}
}

int	pipe_master(char ***cmds, char *files[], char *limit)
{
	int	fds[4];
	int	i[2];

	i[0] = -1;
	get_input_fd(files[PIPEX_IN], &fds[2], &fds[3], limit);
	while (cmds[++(i[0])] != NULL)
	{
		if (pipe(fds) < 0)
			perror(NULL);
		else if (limit && i[0] == 0)
			i[1] = forking_pipe(cmds[i[0]], fds, files, ppx_here_input, limit);
		else if (i[0] == 0)
			i[1] = forking_pipe(cmds[i[0]], fds, files, ppx_file_input, limit);
		else if (limit && cmds[i[0] + 1] == NULL)
			i[1] = forking_pipe(cmds[i[0]], fds, files, ppx_out_append, limit);
		else if (cmds[i[0] + 1] == NULL)
			i[1] = forking_pipe(cmds[i[0]], fds, files, ppx_out_trunc, NULL);
		else
			i[1] = forking_pipe(cmds[i[0]], fds, files, ppx_midpoint, NULL);
		fds[2] = fds[PIPE_READ];
	}
	if (fds[3] >= 0)
		waitpid(i[1], &i[0], 0);
	close(fds[3]);
	return (i[0]);
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
