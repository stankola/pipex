/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bottom_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:19:49 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/29 17:19:50 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"
#include <unistd.h>
#include <sys/wait.h>

void	bottom_duplicator(int input, int output, int errput)
{
	dup2(input, STDIN_FILENO);
	dup2(output, STDOUT_FILENO);
	dup2(errput, STDERR_FILENO);
}

void	bottom_closer_for_middleman(int fd_a, int fd_b, int fd_c)
{
	close(fd_a);
	close(fd_b);
	close(fd_c);
}

int	wait_and_print_errors(int output_fd)
{
	int	status;

	while (wait(&status) >= 0)
	{
		if (WEXITSTATUS(status) != 0)
			print_to_stderr(output_fd);
	}
	return (WEXITSTATUS(status));
}
