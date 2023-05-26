/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:26:23 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/26 15:26:28 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_H
# define PIPEX_H
# ifndef PIPE_READ
#  define PIPE_READ 0
# endif
# ifndef PIPE_WRITE
#  define PIPE_WRITE 1
# endif
# ifndef PIPEX_IN
#  define PIPEX_IN 0
# endif
# ifndef PIPEX_OUT
#  define PIPEX_OUT 1
# endif

// key/value pair
typedef struct	s_pipe_data
{
	int	pid;
	int	pipe_fds[2];
}				t_pipe_data;

enum	e_pipex_cases
{
	ppx_file_input,
	ppx_here_input,
	ppx_midpoint,
	ppx_out_append,
	ppx_out_trunc
};

char		**get_env_path_value(void);

char		*find_cmd(char *exe);

char		*get_full_path(char const *path, char const *file);

int			is_directory(char *file);

void		free_strarray(char ***array);

void		free_strarrayarray(char ****array);

void		read_stdin(char *limiter, int output);

void		pipe_file_input(char **cmds, char *input_file, int output_fd);

void		pipe_file_output_append(char **cmds, int input_fd, char *output_file);

void		pipe_file_output_trunc(char **cmds, int input_fd, char *output_file);

void		pipe_command(char **cmds, int input_fd, int output_fd);

int			check_file_access(char *cmd);

t_pipe_data	*new_pipe_data(int pid, int read_fd, int write_fd);

int			pipe_data_pid_matcher(t_pipe_data *a, int *pid);

#endif
