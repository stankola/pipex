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
# ifndef PIPEX_IN
#  define PIPEX_IN 0
# endif
# ifndef PIPEX_OUT
#  define PIPEX_OUT 1
# endif
# ifndef PIPE_READ
#  define PIPE_READ 0
# endif
# ifndef PIPE_WRITE
#  define PIPE_WRITE 1
# endif
# ifndef PIPE_READ_STDERR
#  define PIPE_READ_STDERR PIPE_READ + 2
# endif
# ifndef PIPE_WRITE_STDERR
#  define PIPE_WRITE_STDERR PIPE_WRITE + 2
# endif
# ifndef INPUT_FD
#  define INPUT_FD 4
# endif
# ifndef OUTPUT_FD
#  define OUTPUT_FD 5
# endif
# include "libft.h"

enum	e_pipex_cases
{
	ppx_file_input,
	ppx_midpoint,
	ppx_out_trunc
};

char		**get_env_path_value(void);

char		*find_cmd(char *exe);

char		*get_full_path(char const *path, char const *file);

int			check_file_access(char *cmd);

int			is_directory(char *file);

void		free_strarray(char ***array);

void		free_strarrayarray(char ****array);

void		pipe_file_input(char **cmds, char *input_file, int output_fd);

void		pipe_file_output_append(char **cmds, int input_fd, char *output_file);

void		pipe_file_output_trunc(char **cmds, int input_fd, char *output_file);

void		pipe_command(char **cmds, int input_fd, int output_fd);

void		copy_int_array(int *to, int *from, int size);

void		replace_fd(char *file, int *fd_ptr, int task);

void		layer_of_pipes(int fds[]);

void		close_extra_pipes(int fd[]);

void		save_process(t_list **process_list, pid_t pid, char *cmd, int err_df);

void		wait_and_print_errors(t_list **process_list);

t_list		*ft_lstgetmatch(t_list **list, int (*f)(void *, void *), void *term);

void		print_to_stderr(int source_fd);

void		bottom_duplicator(int input, int output, int errput);

#endif
