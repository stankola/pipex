/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   middle_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 16:47:59 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/28 16:48:00 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "pipex.h"

void	copy_int_array(int *to, int *from, int size)
{
	while (size-- > 0)
		to[size] = from[size];
}

void	replace_fd(char *file, int *fd_ptr, int task)
{
	int	new_fd;

	new_fd = -1;
	if (task == ppx_file_input)
		new_fd = open(file, O_RDONLY);
	else if (task == ppx_out_trunc)
			new_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP);
	if (new_fd < 0)
	{
		if (errno == 2)
			ft_fprintf(STDERR_FILENO, "no such file or directory: %s\n", file);
		else
			perror(file);
		exit(errno);
	}
	if (*fd_ptr >= 0)
		close(*fd_ptr);
	*fd_ptr = new_fd;
}
