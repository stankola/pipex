/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:25:43 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/26 15:25:45 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "pipex.h"
#include "libft.h"
#ifndef PIPEX_READ_BUFFER_SIZE
# define PIPEX_READ_BUFFER_SIZE 1
#endif

void	read_stdin(char *limiter, int output)
{
	char	*limit_check;
	char	*readbuf;
	int		i;

	readbuf = malloc(sizeof(PIPEX_READ_BUFFER_SIZE));
	while (1)
	{
		limit_check = limiter;
		read(STDIN_FILENO, readbuf, PIPEX_READ_BUFFER_SIZE);
		while (*limit_check != '\0' && *readbuf == *(limit_check))
		{
			*limit_check = *readbuf;
			read(STDIN_FILENO, readbuf, PIPEX_READ_BUFFER_SIZE);
			limit_check++;
		}
		if (*limit_check == '\0')
			break ;
		i = 0;
		while (limit_check != &(limiter[i]))
			write(output, &(limiter[i++]), 1);
		write(output, readbuf, 1);
	}
	free(readbuf);
	exit(0);
}

int	is_directory(char *file)
{
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
