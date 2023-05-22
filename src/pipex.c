/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 18:23:58 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/20 18:23:59 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "libft.h"
#ifndef CHILD_END
# define CHILD_END 1
#endif
#ifndef PARENT_END
# define PARENT_END 0
#endif

char	*get_errmsg()
{
	int		fildes[2];
	int		pid;
	char	*chrbfr;

	chrbfr = malloc(sizeof(char) * 80);
	pipe(fildes);		// Can fail
	pid = fork();
	if (pid > 0)
	{
		close(fildes[CHILD_END]);
		wait(NULL);
		read(fildes[PARENT_END], chrbfr, 80);
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

char	**get_path_env_value()
{
	char	**value;
	char	**iterator;

	iterator = __environ;
	while (*iterator != NULL)
	{
		if (ft_strnstr(*iterator, "PATH", ft_strlen("PATH")))
		{
//			ft_printf("Found somthing %s\nlength %d", *iterator, ft_strlen(key));
//			ft_printf("Value %s\n", &((*iterator)[ft_strlen(key) + 1]));
			value = ft_split(&((*iterator)[ft_strlen("PATH") + 1]), ':');
			return (value);
		}
		iterator++;
	}
	return (NULL);
}

char	*find_path(char *exe)
{
	char	**env_path;
	char	*s;
	char	*path;
	int		i;
	char	**path_iterator;

	ft_printf("finding %s\n", exe);
	env_path = get_path_env_value();
	path = NULL;
	path_iterator = NULL;
	while (path_iterator == NULL || (*path_iterator) != NULL)
	{
		if (access(exe, X_OK))
		{
			s = get_errmsg();
			ft_printf("errmsg %s\n", s);
			if (ft_strnstr(s, "Permission denied", 80))		// is this good? I think the error message might be environment dependent
			{
				free(s);
				return (NULL);
			}
			else if (ft_strnstr(s, "No such file or directory", 80))
			{
				continue ;
			}
			else
				return (s);
			free(s);
		}
		else
		{
			s = ft_strjoin(path, exe);
			free(path);
			return (s);
		}
	}
//		read(fd2, chbuf, 80);

//		ft_printf("OK?\n", chbuf);
//		ft_printf("%s\n", chbuf);
	return (NULL);
}

void	child_laborer(char **cmds, int input, int output)
{
	int	pid;

//	ft_fprintf(STDERR_FILENO, "Child here, input %d output %d cmd %s\n", input, output, cmds[0]);
	close(STDIN_FILENO);
	input = dup(input);
	close(STDOUT_FILENO);
	output = dup(output);
	pid = fork();
	if (pid > 0)
	{
		wait(NULL);
		exit (0);
	}
	else if (pid == 0)
	{
		execve(cmds[0], cmds, __environ);	// Check the environ variable name
		perror("exec fail");
	}
	else
		perror("Child fork fail");
	exit(-1);
}

// returns fd for output
int	do_stuff(int input_fd, int output_fd, char ***cmds)
{
	int	pipe_fds[2];
	int	pid;

	while (*(cmds) != NULL)
	{
		if (pipe(pipe_fds) < 0)
		{
			perror("The pipes break!");
			return (-1);
		}
		pid = fork();
		if (pid > 0)
		{
			close(pipe_fds[CHILD_END]);								// Could fail
//			ft_printf("The wait is over, woken by %d\n", waitpid(pid, NULL, 0));
			input_fd = pipe_fds[PARENT_END];
//			ft_fprintf(STDERR_FILENO, "Parent here, fd %d fd %d\n", pipe_fds[PARENT_END], input_fd);
		}
		else if (pid == 0)
		{
//			ft_printf("Child is %d\n", getpid());
			close(pipe_fds[PARENT_END]);							// Could fail
			if (*(cmds + 1) == NULL)
				child_laborer(*cmds, input_fd, output_fd);
			else
				child_laborer(*cmds, input_fd, pipe_fds[CHILD_END]);
			exit(0);
		}
		else
		{
			perror("The forks break!");
			return (-1);
		}
		cmds++;
	}
	return (input_fd);
}

int	main(int argc, char *argv[])
{
	char	***cmds;
	char	**files;
	int		fd1;
	int		fd2;
	int		i;

	i = 0;
	char **vals = get_path_env_value("PATH");
	while (vals[i] != NULL)
		ft_printf("%s\n", vals[i++]);
//	while (__environ[i] != NULL)			// Check the environ variable name
//		ft_printf("%s\n", __environ[i++]);
//	find_path(argv[2]);
	exit(1012);
	if (argc < 5)
		return (1);	// Some error message here
	cmds = ft_calloc(sizeof(char **), (argc - 2));
	if (cmds == NULL)
		return (1);
	files = malloc(sizeof(char *) * 2);
	if (files == NULL)
	{
		free(cmds);
		return (1);
	}
	i = 1;
	while (++i < argc - 1)
		cmds[i - 2] = ft_split(argv[i], ' ');
	files[0] = argv[1];
	files[1] = argv[argc - 1];
	fd1 = open(files[0], O_RDONLY);
	fd2 = open(files[1], O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP);
//	char *chbuf = ft_calloc(sizeof(char), (80));
	int fd3 = do_stuff(fd1, fd2, cmds);
	ft_printf("fd1 %d fd2 %d fd3 %d\n", fd1, fd2, fd3);
//	int chars_read = read(fd3, chbuf, 80);
//	ft_printf("Read %d characters\n", chars_read);
//	chars_read = write(fd2, chbuf, chars_read);
//	ft_printf("Wrote %d characters\n", chars_read);
	return (0);
}
