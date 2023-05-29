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
# define PIPEX_IN 0
# define PIPEX_OUT 1
# define PIPE_READ 0
# define PIPE_WRITE 1
# define PIPE_READ_STDERR 2
# define PIPE_WRITE_STDERR 3
# define INPUT_FD 4
# define OUTPUT_FD 5
# include "libft.h"

enum	e_pipex_cases
{
	ppx_file_input,
	ppx_here_input,
	ppx_midpoint,
	ppx_out_append,
	ppx_out_trunc
};

char		***get_cmds(char *argv[], int argc);

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

int			layer_of_pipes(int fds[]);

void		close_extra_pipes(int fd[]);

void		save_process(t_list **process_list, pid_t pid, char *cmd,
				int err_df);

int			wait_and_print_errors(int output_fd);

void		wait_for_processes_to_end(t_list **process_list);

t_list		*ft_lstgetmatch(t_list **list, int (*f)(void *, void *),
				void *term);

void		print_to_stderr(int source_fd);

void		bottom_duplicator(int input, int output, int errput);

void		bottom_closer_for_middleman(int fd_a, int fd_b, int fd_c);

#endif
