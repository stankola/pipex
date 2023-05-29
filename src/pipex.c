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
/*
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
}*/
/*
// Not finished
t_process_header	*get_process_header_by_pid(t_list **list, pid_t pid)
{
	t_list	*iterator;
	t_list	*temp_list;
	t_process_header	*ph;

	iterator = *list;
	if (*list == NULL)
		return (NULL);
	if (((t_process_header *)iterator->content)->pid == pid)
		(*list) = (*list)->next;
	else
	{
		while (iterator->next != NULL)
		{
			if (((t_process_header *)iterator->next->content)->pid == pid)
				break ;
			iterator = iterator->next;
		}
	}
	if (iterator->next == NULL && ((t_process_header *)iterator->content)->pid != pid)
		return (NULL);
	temp_list = iterator->next;
	iterator->next = temp_list->next;
	ph = (t_process_header *)temp_list->content;
	free(temp_list);
	return (ph);
}*/
/*
// Removes the first matching item from the list and returns it.
t_list	*ft_lstgetmatch(t_list **list, int (*f)(void *, void *), void *term)
{
	t_list	*iterator;
	t_list	*temp_list;

	if (*list == NULL)
		return (NULL);
	iterator = *list;
	if (f(iterator->content, term))
	{
		*list = (*list)->next;
		iterator->next = NULL;
		return (iterator);
	}
	while (iterator->next != NULL)
	{
		if (f(iterator->next->content, term))
		{
			temp_list = iterator;
			iterator = iterator->next;
			temp_list->next = temp_list->next->next;
			iterator->next = NULL;
			return (iterator);
		}
		else
			iterator = iterator->next;
	}
	return (NULL);
}*/
/*
void	wait_and_print_errors(t_list **list)
{
	int		stat_loc;
	pid_t	pid;
	t_list	*temp_list;
	t_process_header	*ph;

	while (1)
	{
		pid = wait(&stat_loc);
		if (pid < 0)
			break ;
		ft_fprintf(STDERR_FILENO, "%p %p\n", &pid, (void *)&pid);
		temp_list = ft_lstgetmatch(list, (int (*)(void *, void *))match_process_header_to_pid, (void *)&pid);
		if (temp_list == NULL)
			ft_fprintf(STDERR_FILENO, "OH no\n");
		ph = (t_process_header *)temp_list->content;
		free(temp_list);
		if (WEXITSTATUS(stat_loc) > 0)
		{
			ft_fprintf(STDERR_FILENO, "%s : %s\n", ph->cmd, strerror(WEXITSTATUS(stat_loc)));
		}
		free(ph);
	}
}*/
/*
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
//		ft_lstadd_front(&list, ft_lstnew(new_process_header(i[1], cmds[i[0]][0], files)));
	}
	close(fds[2]);
//	wait_and_print_errors(&list);
//	while (wait(NULL) >= 0)
//		;
}*/

// Bottom worker executes commands
int	bottom_worker(char **cmd, int input_fd, int output_fd)
{
	char	*exe;
	extern char	**environ;
	pid_t	pid;
	int		status;
	int		middle_fd[2];

	exe = find_cmd(cmd[0]);
	pipe(middle_fd);
	pid = fork();
	if (pid == 0)
	{
		close(middle_fd[PIPE_READ]);
//		ft_printf("Bottom: input %d output %d errput %d\n", input_fd, output_fd, middle_fd[PIPE_WRITE]);
		bottom_duplicator(input_fd, output_fd, middle_fd[PIPE_WRITE]);
		if (check_file_access(exe))
			execve(exe, cmd, environ);
		exit(errno);
	}
	close(middle_fd[PIPE_WRITE]);
//	ft_printf("Middle: started pid %d\n", pid);
//	ft_printf("Middle: closing %d %d\n", input_fd, output_fd);
	close(input_fd);
	close(output_fd);
	status = 0;
	if (pid > 0)
	{
		waitpid(pid, &status, 0);
		status = WEXITSTATUS(status);
		if (status > 0)
			print_to_stderr(middle_fd[PIPE_READ]);
	}
	close (middle_fd[PIPE_READ]);
//	ft_fprintf(STDERR_FILENO, "pid %d EXE is %s\n", pid, exe);
	free(exe);
	if (pid < 0)
		status = (-1);
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
		else if (task == ppx_out_trunc)
			replace_fd(files[PIPEX_OUT], &local_fds[OUTPUT_FD], task);
		status = bottom_worker(cmd, local_fds[INPUT_FD], local_fds[OUTPUT_FD]);
		exit(status);
	}
	return (pid);
}

// Top executive handles pipelines
void	top_executive(char ***cmds, char *files[])
{
	int		i;
	int		fds[6];	// {PIPE_READ, PIPE_WRITE, PIPE2_READ, PIPE2_WRITE, (input fd)Prev PIPE_READ, ?Output fd?}
	pid_t	pid;
	t_list	*process_list;

	i = -1;
	process_list = NULL;
	fds[INPUT_FD] = -1;
	while (cmds[++i] != NULL)
	{
		layer_of_pipes(fds);
		if (i == 0)
			pid = middle_management(cmds[i], fds, files, ppx_file_input);
		else if (cmds[i + 1] == NULL)
			pid = middle_management(cmds[i], fds, files, ppx_out_trunc);
		close_extra_pipes(fds);
		save_process(&process_list, pid, cmds[i][0], fds[PIPE_READ_STDERR]);
	}
	close(fds[INPUT_FD]);
	wait_and_print_errors(&process_list);
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
	io_files[PIPEX_IN] = argv[1];
	cmds = get_cmds(&argv[2], argc - 3);
	io_files[PIPEX_OUT] = argv[argc - 1];
	top_executive(cmds, io_files);
	free_strarrayarray(&cmds);
	return (0);
}
