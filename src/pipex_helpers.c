#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "pipex.h"
#include "libft.h"

char	**get_env_path_value()
{
	char	**value;
	char	**iterator;
	extern char **environ;

	iterator = environ; /// babababa
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
	{
		if (access(exe, F_OK) == 0)
			return (ft_strdup(exe));
		return (NULL);
	}
	env_path = get_env_path_value();
	path_iterator = env_path;
	while (*path_iterator != NULL)
	{
		full_location = get_full_path(*(path_iterator++), exe);
		if (access(full_location, F_OK) == 0)		// found
			if (! is_directory(full_location))
			{
				free_strarray(&env_path);
				return (full_location);
			}
		free(full_location);
	}
	free_strarray(&env_path);
	return (NULL);
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
