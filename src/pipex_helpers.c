/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 23:25:52 by tsankola          #+#    #+#             */
/*   Updated: 2023/07/03 15:49:31 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "pipex.h"
#ifndef CBUF_LENGTH
# define CBUF_LENGTH 256
#endif

int	is_directory(char *file)
{
	// This implementation might actually be wrong. O_DIRECTORY is not supported
	// in BSD / MacOS documentation (even though it works in my tests). Instead
	// it would be better to open the file with O_WRONLY flag and then check
	// for errno value EISDIR / 21.
	int	fd;

	fd = open(file, O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		return (1);
	}
	return (0);
}

void	free_strarray(char ***array)
{
	char	**i;

	i = *array;
	while (*i != NULL)
		free(*(i++));
	free(*array);
	*array = NULL;
}

void	free_strarrayarray(char ****array)
{
	char	***i;

	i = *array;
	while (*i != NULL)
	{
		free_strarray(i);
		i++;
	}
	free(*array);
	*array = NULL;
}

void	print_to_stderr(int source_fd)
{
	char	cbuf[CBUF_LENGTH];
	int		c_count;

	c_count = read(source_fd, cbuf, CBUF_LENGTH);
	if (c_count > 0)
	{
		cbuf[c_count - 1] = '\0';
		ft_fprintf(STDERR_FILENO, "%s\n", cbuf);
	}
}

void	pipex_print_error(int errnum, char *s)
{
	char	*output;

	output = strerror(errnum);
	ft_fprintf(STDERR_FILENO, "%s: %s\n", output, s);
}
