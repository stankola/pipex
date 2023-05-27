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
