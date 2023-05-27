/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_proc_hdr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:40:20 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/27 22:40:21 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "pipex_proc_hdr.h"

t_process_header	*new_process_header(pid_t pid, char *cmd, char **files)
{
	t_process_header	*ph;
	ph = malloc(sizeof(t_process_header));
	if (ph == NULL)
		return (NULL);
	ph->pid = pid;
	ph->cmd = cmd;
	ph->files = files;
	return (ph);
}

int	match_process_header_to_pid(t_process_header *ph, pid_t *pid)
{
	if (ph->pid == *pid)
		return (1);
	return (0);
}

void	del_process_header(t_process_header **ph)
{
	(*ph)->pid = 0;
	(*ph)->cmd = NULL;
	(*ph)->files = NULL;
	free(*ph);
	*ph = NULL;
}
