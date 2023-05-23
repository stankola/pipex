#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include "pipex.h"
#include "libft.h"
#ifndef ERR_MSG_LENGTH
# define ERR_MSG_LENGTH 80
#endif

char	*get_errmsg()
{
	int		fildes[2];
	int		pid;
	char	*chrbfr;

	chrbfr = malloc(sizeof(char) * ERR_MSG_LENGTH);
	pipe(fildes);		// Can fail
	pid = fork();
	if (pid > 0)
	{
		close(fildes[CHILD_END]);
		wait(NULL);
		read(fildes[PARENT_END], chrbfr, ERR_MSG_LENGTH);
		return (chrbfr);
	}
	else if (pid == 0)
	{
		close(fildes[PARENT_END]);
		close(STDERR_FILENO);
		dup(fildes[CHILD_END]);
		perror(NULL);
		exit(0);
	}
	else
		perror("Forking hell, I don't know what to do!");
	return (NULL);
}

char	**get_env_path_value()
{
	char	**value;
	char	**iterator;

	iterator = __environ;
	while (*iterator != NULL)
	{
		if (ft_strnstr(*iterator, "PATH", ft_strlen("PATH")))
		{
//			ft_fprintf(STDERR_FILENO, "%s\n", *iterator);
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
		if (access(exe, F_OK) == 0)
			return (ft_strdup(exe));
	env_path = get_env_path_value();
	path_iterator = env_path;
	while (*path_iterator != NULL)
	{
		full_location = get_full_path(*(path_iterator++), exe);
//		ft_printf("Trying %s\n", full_location);
		if (access(full_location, F_OK) == 0)		// found
		{
			if (is_directory(full_location))
				continue ;
			free(env_path);
			return (full_location);
		}
	}
	free(env_path);		//Memory leak waiting to be fixed
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

int	check_file_execution_access(char *location)
{
	char	*s;

	if (access(location, X_OK))
	{
		s = get_errmsg(); // is this bad? I think the error message might be environment dependent
		if (! ft_strnstr(s, "No such file or directory", ERR_MSG_LENGTH))
		{
			ft_fprintf(STDERR_FILENO, "%s: %s\n", location, s);
			free(s);
			return(-1);
		}
		free(s);
		return (-2);
	}
	return (0);
}

int	is_directory(char *file)
{
	int	fd;

	fd = open(file, __O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		return (1);
	}
	return (0);
}