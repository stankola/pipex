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
#include <errno.h>
#include "libft.h"
#include "pipex.h"
#include "pipex_proc_hdr.h"

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

int	forking_pipe(char **cmds, int fds[], char *files[], int pipe_case)
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
		else if (pipe_case == ppx_midpoint)
			pipe_command(cmds, local_fds[2], local_fds[PIPE_WRITE]);
		else if (pipe_case == ppx_out_trunc)
			pipe_file_output_trunc(cmds, local_fds[2], files[PIPEX_OUT]);
	}
	else if (pid < 0)
		perror(NULL);
	close(fds[PIPE_WRITE]);
	return (pid);
}

void	wait_and_print_errors(t_list *list)
{
	int		stat_loc;
	pid_t	pid;

	list +=0;
	while (1)
	{
		pid = wait(&stat_loc);
		if (pid < 0)
			break ;
		if (WEXITSTATUS(stat_loc) > 0)
		{
			// FIND header with pid == pid and add cmd string to error message.	
			// There's the problem that missing io file and cmd both return the same error number.
			// Perhaps I should implement my own for the missing io files.
			ft_fprintf(STDERR_FILENO, "pid %d %s\n", pid, strerror(WEXITSTATUS(stat_loc)));
		}
	}
}

void	pipe_master(char ***cmds, char *files[])
{
	int		fds[4];
	int		i[2];
	t_list	*list;		// change to ring?

	i[0] = -1;
	list = NULL;
	while (cmds[++(i[0])] != NULL)
	{
		if (pipe(fds) < 0)
			perror(NULL);
		else if (i[0] == 0)
			i[1] = forking_pipe(cmds[i[0]], fds, files, ppx_file_input);
		else if (cmds[i[0] + 1] == NULL)
			i[1] = forking_pipe(cmds[i[0]], fds, files, ppx_out_trunc);
		else
			i[1] = forking_pipe(cmds[i[0]], fds, files, ppx_midpoint);
		if (i[0] != 0)
			close(fds[2]);
		fds[2] = fds[PIPE_READ];
		ft_lstadd_front(&list, ft_lstnew(new_process_header(i[1], cmds[i[0]][0], files)));
	}
	close(fds[2]);
	wait_and_print_errors(list);
//	while (wait(NULL) >= 0)
//		;
}

int	main(int argc, char *argv[])
{
	char	***cmds;
	char	*io_files[2];

	if (argc < 5)
	{
		ft_fprintf(STDERR_FILENO, "Wrong number of arguments %d\n", argc);
		return (-1);
	}
	io_files[PIPEX_OUT] = argv[argc - 1];
	io_files[PIPEX_IN] = argv[1];
	cmds = get_cmds(&argv[2], argc - 3);
	pipe_master(cmds, io_files);
	free_strarrayarray(&cmds);
	return (0);
}
