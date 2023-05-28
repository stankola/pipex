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
#include "pipex.h"
#include "libft.h"
#include "pipex_proc_hdr.h"

static void	layer_of_pipes(int fds[])
{
	pipe(fds);
	pipe(&fds[STDERR_READ]);	// these might fail
	fds[OUTPUT_FD] = fds[PIPE_WRITE];
}

void	closing_time(int fds[])
{
	fds[INPUT_FD] = fds[PIPE_READ];
	close(fds[OUTPUT_FD]);
	close(fds[STDERR_WRITE]);
}

void	save_process(t_list **process_list, pid_t pid, char *cmd, int err_fd)
{
	t_process_header	*ph;
	t_list				*newlist;

	ph = new_process_header(pid, cmd, err_fd);
	if (ph == NULL)
		return ;
	newlist = ft_lstnew(ph);
	if (newlist != NULL)
		ft_lstadd_front(process_list, ft_lstnew(ph));
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
			read(ph->err_fd, buf, 79);
			ft_fprintf(STDERR_FILENO, "%s", buf);
		}
		free(ph);
		pid = wait(&status);
	}
}