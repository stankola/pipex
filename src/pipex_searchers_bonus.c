/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_searchers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 23:26:10 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/26 23:26:19 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "pipex_bonus.h"

char	**get_env_path_value(void)
{
	char		**value;
	char		**iterator;
	extern char	**environ;

	iterator = environ;
	while (*iterator != NULL)
	{
		if (ft_strnstr(*iterator, "PATH", ft_strlen("PATH")))
		{
			value = ft_split(&((*iterator)[ft_strlen("PATH") + 1]), ':');
			return (value);
		}
		iterator++;
	}
	return (NULL);
}

char	*find_cmd(char *exe)
{
	char	**env_path;
	char	**path_iterator;
	char	*full_location;

	if (ft_strchr(exe, '/'))
		return (ft_strdup(exe));
	env_path = get_env_path_value();
	path_iterator = env_path;
	while (*path_iterator != NULL)
	{
		full_location = get_full_path(*(path_iterator++), exe);
		if (access(full_location, F_OK) == 0)
		{
			if (is_directory(full_location) == 0)
			{
				free_strarray(&env_path);
				return (full_location);
			}
		}
		free(full_location);
	}
	free_strarray(&env_path);
	return (ft_strdup(exe));
}

char	*get_full_path(char const *path, char const *file)
{
	char	*result;
	int		resultlength;

	if (path == NULL)
		return (ft_strdup(file));
	if (path[ft_strlen(path) - 1] == '/')
		result = ft_strjoin(path, file);
	else
	{
		resultlength = ft_strlen(path) + ft_strlen(file) + 1;
		result = malloc(sizeof(char) * (resultlength + 1));
		ft_strlcpy(result, path, resultlength + 1);
		result[ft_strlen(path)] = '/';
		result[ft_strlen(path) + 1] = '\0';
		ft_strlcat(result, file, resultlength + 1);
	}
	return (result);
}

int	check_file_access(char *cmd)
{
	if (access(cmd, F_OK) != 0)
	{
		if (ft_strchr(cmd, '/'))
			ft_fprintf(STDERR_FILENO, "no such file or directory: %s\n", cmd);
		else
			ft_fprintf(STDERR_FILENO, "command not found: %s\n", cmd);
		return (0);
	}
	return (1);
}
