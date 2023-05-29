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
#include "pipex.h"
#include "libft.h"
#include "pipex_proc_hdr.h"

void	layer_of_pipes(int fds[])
{
	pipe(fds);
	pipe(&fds[PIPE_READ_STDERR]);	// these might fail
//	ft_printf("Top: Opened pipe fds read %d write %d err_read %d err_write %d\n", fds[0], fds[1], fds[2], fds[3]);
	fds[OUTPUT_FD] = fds[PIPE_WRITE];
}

void	close_extra_pipes(int fds[])
{
//	ft_printf("Top: Closing fds");
	if (fds[INPUT_FD] >= 0)
	{
//		ft_printf(" %d", fds[INPUT_FD]);
		close(fds[INPUT_FD]);
	}
	fds[INPUT_FD] = fds[PIPE_READ];
//	ft_printf(" %d", fds[PIPE_WRITE]);
	close(fds[PIPE_WRITE]);
//	ft_printf(" %d\n", fds[PIPE_WRITE_STDERR]);
	close(fds[PIPE_WRITE_STDERR]);
}

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
}

void	save_process(t_list **process_list, pid_t pid, char *cmd, int err_fd)
{
	t_process_header	*ph;
	t_list				*newlist;
//	ft_printf("Top: saving fd %d\n", err_fd);
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

void	wait_and_print_errors(t_list **process_list)
{
	pid_t				pid;
	int					status;
	t_process_header	*ph;
	t_list				*temp_list;
	char				buf[80];

	buf[79] = '\0';
	pid = wait(&status);
	while (pid >= 0)
	{
		temp_list = ft_lstgetmatch(process_list, (int (*)(void *, void *))match_process_header_to_pid, (void *)&pid);
		ph = (t_process_header *)temp_list->content;
		free(temp_list);
		if (WEXITSTATUS(status) > 0)
		{
			print_to_stderr(ph->err_fd);
//			ft_fprintf(STDERR_FILENO, "pid %d exit %d\n", pid, WEXITSTATUS(status));
//			int chr = read(ph->err_fd, buf, 79);
//			buf[chr - 1] = '\0';
//			ft_fprintf(STDERR_FILENO, "%s\n", buf);
		}
//		ft_printf("Top: closing %d\n", ph->err_fd);
		close(ph->err_fd);
//		getchar();
		free(ph);
		pid = wait(&status);
	}
}
