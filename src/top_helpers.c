/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   top_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 16:50:00 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/28 16:50:01 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "pipex.h"
#include "pipex_proc_hdr.h"

int	layer_of_pipes(int fds[])
{
	int	errcheck;

	errcheck = pipe(fds);
	if (errcheck < 0)
		return (-1);
	errcheck = pipe(&fds[PIPE_READ_STDERR]);
	if (errcheck < 0)
	{
		close(fds[PIPE_READ]);
		close(fds[PIPE_WRITE]);
		return (-1);
	}
	fds[OUTPUT_FD] = fds[PIPE_WRITE];
	return (0);
}

void	close_extra_pipes(int fds[])
{
	if (fds[INPUT_FD] >= 0)
		close(fds[INPUT_FD]);
	fds[INPUT_FD] = fds[PIPE_READ];
	close(fds[PIPE_WRITE]);
	close(fds[PIPE_WRITE_STDERR]);
}

// Removes the first matching item from the list and returns it.
t_list	*ft_lstgetmatch(t_list **list, int (*f)(void *, void *), void *term)
{
	t_list	*iterator;
	t_list	*match;

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
			match = iterator->next;
			iterator->next = iterator->next->next;
			match->next = NULL;
			return (match);
		}
		else
			iterator = iterator->next;
	}
	return (NULL);
}

void	save_process(t_list **process_list, pid_t pid, char *cmd, int err_fd)
{
	t_process_header	*ph;
	t_list				*newlist;

	if (pid < 0)
	{
		close(err_fd);
		return ;
	}
	ph = new_process_header(pid, cmd, err_fd);
	if (ph == NULL)
		return ;
	newlist = ft_lstnew(ph);
	if (newlist == NULL)
	{
		free(ph);
		return ;
	}
	ft_lstadd_front(process_list, newlist);
	if (*process_list == NULL)
	{
		free(ph);
		free(newlist);
	}
}

int	wait_for_processes_to_end(t_list **process_list)
{
	pid_t				pid[2];
	int					status;
	t_process_header	*ph;
	t_list				*temp_list;
	int					last_value;

	last_value = 0;
	pid[1] = ((t_process_header *)(*process_list)->content)->pid;
	pid[0] = wait(&status);
	while (pid[0] >= 0)
	{
		temp_list = ft_lstgetmatch(process_list,
				(int (*)(void *, void *))match_process_header_to_pid,
				(void *)&pid[0]);
		ph = (t_process_header *)temp_list->content;
		free(temp_list);
		if (ph->pid == pid[1])
			last_value = WEXITSTATUS(status);
		if (WEXITSTATUS(status) > 0)
			print_to_stderr(ph->err_fd);
		close(ph->err_fd);
		free(ph);
		pid[0] = wait(&status);
	}
	return (last_value);
}
