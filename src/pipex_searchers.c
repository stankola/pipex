/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_searchers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 23:26:10 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/26 23:26:19 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "libft.h"
#include "pipex.h"
#include <stdio.h>

char	***get_cmds(char *argv[], int argc)
{
	int		i;
	char	***cmds;

	i = 0;
	cmds = ft_calloc(sizeof(char **), (argc + 1));
	while (i < argc)
	{
		cmds[i] = ft_split(argv[i], ' ');
		i++;
	}
/*ft_printf("%s %s\n", cmds[0][0], argv[0]);
	ft_printf("%p\n", *cmds[0]);
	if (cmds[0] == NULL)
		ft_printf("well\n");
//	char *s = find_cmd(*cmds[0]);
//	ft_printf("%p\n", s);
		ft_printf("cmd %s\n", find_cmd(cmds[0][0]));
	if (access(cmds[0][0], F_OK) != 0)
	{
		ft_printf("cmd %s not found: ", cmds[0][0]);
		perror(NULL);
	}
	exit(1);*/
	return (cmds);
}

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

	if (exe == NULL)
		return (NULL);
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
		if (result == NULL)
			return (NULL);
		ft_strlcpy(result, path, resultlength + 1);
		result[ft_strlen(path)] = '/';
		result[ft_strlen(path) + 1] = '\0';
		ft_strlcat(result, file, resultlength + 1);
	}
	return (result);
}

void	check_file_access(char *cmd)
{
	if (cmd == NULL)
	{
		ft_fprintf(STDERR_FILENO, "command not found: \0\n");
		exit (127);
	}
	if (access(cmd, F_OK) != 0)
	{
		if (ft_strchr(cmd, '/'))
			pipex_print_error(errno, cmd);
		else
		{
			if (cmd == NULL)
				ft_fprintf(STDERR_FILENO, "command not found: %c\n", '\0');
			else
				ft_fprintf(STDERR_FILENO, "command not found: %s\n", cmd);
		}
		exit (127);
	}
	if (access(cmd, X_OK) != 0)
	{
		pipex_print_error(errno, cmd);
		exit (errno);
	}
}
