/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 23:25:34 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/26 23:25:45 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "libft.h"
#include "pipex.h"
#include "pipex_proc_hdr.h"

// Bottom worker executes commands
int	bottom_worker(char **cmd, int input_fd, int output_fd)
{
	char		*exe;
	pid_t		pid;
	int			status;
	int			middle_fd[2];

	status = pipe(middle_fd);
	if (status < 0)
		return (status);
	exe = find_cmd(cmd[0]);
	pid = fork();
	if (pid == 0)
		bottom_work(cmd, exe, middle_fd, (int []){input_fd, output_fd});
	close(middle_fd[PIPE_WRITE]);
	if (pid > 0)
		status = wait_and_print_errors(middle_fd[PIPE_READ]);
	else
		status = (-1);
	close (middle_fd[PIPE_READ]);
	free(exe);
	return (status);
}

// Middle management handles redirections ie. sets up file io
pid_t	middle_management(char **cmd, int fds[], char *files[], int task)
{
	int		local_fds[6];
	pid_t	pid;
	int		status;

	copy_int_array(local_fds, fds, 6);
	pid = fork();
	if (pid == 0)
	{
		dup2(local_fds[PIPE_WRITE_STDERR], STDERR_FILENO);
		close(local_fds[PIPE_READ]);
		close(local_fds[PIPE_READ_STDERR]);
		if (task == ppx_file_input)
			replace_fd(files[PIPEX_IN], &local_fds[INPUT_FD], task);
		else if (task == ppx_out_trunc || task == ppx_out_append)
			replace_fd(files[PIPEX_OUT], &local_fds[OUTPUT_FD], task);
		status = bottom_worker(cmd, local_fds[INPUT_FD], local_fds[OUTPUT_FD]);
		close(local_fds[INPUT_FD]);
		close(local_fds[OUTPUT_FD]);
		exit(status);
	}
	return (pid);
}

pid_t	executive_decision(int i, char ***cmds, int fds[], char *files[])
{
	pid_t	pid;

	if (i == 0)
		pid = middle_management(cmds[i], fds, files, ppx_file_input);
	else if (cmds[i + 1] == NULL)
		pid = middle_management(cmds[i], fds, files, ppx_out_trunc);
	else
		pid = middle_management(cmds[i], fds, files, ppx_midpoint);
	return (pid);
}

// Top executive handles pipelines
int	top_executive(char ***cmds, char *files[])
{
	int		i;
	int		fds[6];
	pid_t	pid;
	t_list	*process_list;

	i = -1;
	pid = -1;
	process_list = NULL;
	fds[INPUT_FD] = -1;
	while (cmds[++i] != NULL && layer_of_pipes(fds) >= 0)
	{
		pid = executive_decision(i, cmds, fds, files);
		close_extra_pipes(fds);
		save_process(&process_list, pid, cmds[i][0], fds[PIPE_READ_STDERR]);
	}
	close(fds[INPUT_FD]);
	return (wait_for_processes_to_end(&process_list));
}

int	main(int argc, char *argv[])
{
	char	***cmds;
	char	*io_files[2];
	int		return_value;

	if (argc != 5)
	{
		ft_fprintf(STDERR_FILENO, "Wrong number of arguments %d\n", argc - 1);
		return (-1);
	}
	cmds = get_cmds(&argv[2], argc - 3);
	io_files[PIPEX_IN] = argv[1];
	io_files[PIPEX_OUT] = argv[argc - 1];
	return_value = top_executive(cmds, io_files);
	free_strarrayarray(&cmds);
	return (return_value);
}
