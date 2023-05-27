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

typedef struct	s_process_header
{
	pid_t	pid;
	char	*cmd;
	char	**files;
}				t_process_header;

#endif
