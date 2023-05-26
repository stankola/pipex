/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 23:26:35 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/26 23:26:49 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
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

int			check_file_access(char *cmd);

void		read_stdin(char *limiter, int output);

int			is_directory(char *file);

void		free_strarray(char ***array);

void		free_strarrayarray(char ****array);

void		pipe_file_input(char **cmds, char *input_file, int output_fd);

void		pipe_file_output_append(char **cmds, int input_fd, char *output_file);

void		pipe_file_output_trunc(char **cmds, int input_fd, char *output_file);

void		pipe_command(char **cmds, int input_fd, int output_fd);

#endif
