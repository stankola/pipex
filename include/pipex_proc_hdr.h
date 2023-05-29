/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_proc_hdr.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:40:13 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/27 22:40:14 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_PROC_HDR_H
# define PIPEX_PROC_HDR_H
# include <sys/wait.h>

typedef struct s_process_header
{
	pid_t	pid;
	char	*cmd;
	char	err_fd;
}				t_process_header;

t_process_header	*new_process_header(pid_t pid, char *cmd, int err_fd);

int					match_process_header_to_pid(t_process_header *ph,
						pid_t *pid);

void				del_process_header(t_process_header **ph);

#endif
