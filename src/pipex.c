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
#include <sys/stat.h>
#include "libft.h"
#ifndef CHILD_END
# define CHILD_END 1
#endif
#ifndef PARENT_END
# define PARENT_END 0
#endif

// Probably useless.
void	append_string_array(char **arr[], char *str)
{
	char	**new_arr;
	int		size;
	int		i;

	size = 0;
	while ((*arr)[size] != NULL)
		size++;
	new_arr = malloc(sizeof(char **) * size + 2);
	if (new_arr == NULL)
		return ;
	i = 0;
	while (i < size)
	{
		new_arr[i] = (*arr)[i];
		ft_printf("%s\n", new_arr[i]);
		i++;
	}
	new_arr[i++] = str;
	new_arr[i] = NULL;
	free(*arr);
	*arr = new_arr;
}

void	child(int fildes)
{
	close(fildes);
	exit(EXIT_SUCCESS);
}

void	parent(int fildes)
{
	char	*buff;
	int		rdcount;
	pid_t	pid;

	buff = malloc (sizeof(char) * 80);
//	ft_fprintf(fildes,"I am not a child!\n");
	pid = wait(NULL);
	ft_printf("The wait is over, woken by %d\n", pid);
	rdcount = read(fildes, buff, 80);
	ft_printf("Read %d chars\n", rdcount);
	ft_printf("This is not a child, I hear %s\n", buff);
	free(buff);

}

char	*run_cmd(char *cmd, char *argv[], int fd)
{
	pid_t	pid;
	int		fildes[2];

	if (pipe(fildes) < 0)
	{
		perror("Piping failure");
		return (NULL);
	}
	pid = fork();
	if (pid == 0)
	{
		ft_printf("Child is %d fd %d\n", getpid(), fildes[CHILD_END]);
		close(fildes[PARENT_END]);
		if (dup2(fildes[CHILD_END], STDOUT_FILENO) < 0 || dup2(fd, STDIN_FILENO) < 0)
			return (NULL);	// How to properly handle this situation? Should fildes[CHILD_END] be closed?
		execve(cmd, argv, NULL);
		perror(NULL);
	}
	else if (pid > 0)
	{
		close(fildes[CHILD_END]);
		parent(fildes[PARENT_END]);
	}
	else
		perror("Forking failure");
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
//		close(output);
//		close(input);
		exit (0);
	}
	else if (pid == 0)
	{
		execve(cmds[0], cmds, NULL);
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
