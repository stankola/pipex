#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "pipex.h"
#include "libft.h"

static void	pipe_fork(char *cmd, char **cmds, int input, int output)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid > 0)
	{
		if (waitpid(pid, &status, 0) < 0)
			perror(cmd);
		free(cmd);
		exit (0);
	}
	else if (pid == 0)
	{
		dup2(input, STDIN_FILENO);
		dup2(output, STDOUT_FILENO);
		if (access(cmd, X_OK) == 0)
			execve(cmd, cmds, __environ);	// Check the environ variable name
//		perror(cmd);
		exit(-1);
	}
	else
		perror(NULL);
	free(cmd);
	exit(-1);
}

void	pipe_file_input(char **cmds, char *input_file, int output_fd)
{
	int		input_fd;

	input_fd = open(input_file, O_RDONLY);
	if (input_fd < 0)
		perror(input_file);
	else
		pipe_command(cmds, input_fd, output_fd);
	exit(-1);
}

void	pipe_file_output_append(char **cmds, int input_fd, char *output_file)
{
	int		output_fd;

	output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND,
		S_IRUSR| S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP);
	if (output_fd < 0)
		perror(output_file);
	else
		pipe_command(cmds, input_fd, output_fd);
	exit(-1);
}

void	pipe_file_output_trunc(char **cmds, int input_fd, char *output_file)
{
	int		output_fd;

	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP);
	if (output_fd < 0)
		perror(output_file);
	else
		pipe_command(cmds, input_fd, output_fd);
	exit(-1);
}

void	pipe_command(char **cmds, int input_fd, int output_fd)
{
	char	*cmd;

	cmd = find_cmd(cmds[0]);
	if (cmd != NULL)
		pipe_fork(cmd, cmds, input_fd, output_fd);
	perror(cmds[0]);
	exit(-1);
}
