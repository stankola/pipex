/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:25:34 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/26 15:25:36 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include "pipex.h"
#include "libft.h"

static void	cmd_fork(char *cmd, char **cmds, int input, int output)
{
	int			pid;
	extern char	**environ;
	int			status;

	dup2(input, STDIN_FILENO);
	dup2(output, STDOUT_FILENO);
	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &status, 0);
//		ft_fprintf(STDERR_FILENO, "errno %d status %d\n", errno, WEXITSTATUS(status));
		free(cmd);
		exit (WEXITSTATUS(status));
	}
	else if (pid == 0)
	{
		close(STDERR_FILENO);
//		if (check_file_access(cmd))
			execve(cmd, cmds, environ);
		exit(errno);
	}
	perror(NULL);																	// don't print error, exit with error code.
	free(cmd);
	exit(errno);
}

void	pipe_file_input(char **cmds, char *input_file, int output_fd)
{
	int	input_fd;

	input_fd = open(input_file, O_RDONLY);
	if (input_fd < 0)
		ft_fprintf(STDERR_FILENO, "no such file or directory: %s\n", input_file);		// don't print error, exit with error code.
	else
		pipe_command(cmds, input_fd, output_fd);
	exit(-1);
}

void	pipe_file_output_append(char **cmds, int input_fd, char *output_file)
{
	int	output_fd;

	output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND,
			S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP);
	if (output_fd < 0)
		perror(output_file);															// don't print error, exit with error code.
	else
		pipe_command(cmds, input_fd, output_fd);
	exit(-1);
}

void	pipe_file_output_trunc(char **cmds, int input_fd, char *output_file)
{
	int	output_fd;

	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP);
	if (output_fd < 0)
		perror(output_file);															// don't print error, exit with error code.
	else
		pipe_command(cmds, input_fd, output_fd);
	exit(-1);
}

void	pipe_command(char **cmds, int input_fd, int output_fd)
{
	char	*cmd;

	cmd = find_cmd(cmds[0]);
	if (cmd != NULL)
		cmd_fork(cmd, cmds, input_fd, output_fd);
	perror(cmds[0]);																	// don't print error, exit with error code.
	exit(-1);
}
