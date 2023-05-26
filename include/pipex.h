// TODO header

#ifndef PIPEX_H
# define PIPEX_H
# ifndef PIPE_READ
#  define PIPE_READ 0
# endif
# ifndef PIPE_WRITE
#  define PIPE_WRITE 1
# endif
#ifndef PIPEX_IN
# define PIPEX_IN 0
#endif
#ifndef PIPEX_OUT
# define PIPEX_OUT 1
#endif

enum	e_pipe_cases
{
	ppx_file_input,
	ppx_here_input,
	ppx_midpoint,
	ppx_out_append,
	ppx_out_trunc
};

typedef struct	s_pipex
{
	char	**cmds;
	int		*fds;
	char	**files;
	char	*limit;
	int		position;
}				t_pipex;

char	**get_env_path_value();

char	*find_cmd(char *exe);

char	*get_full_path(char const *path, char const *file);

int		is_directory(char *file);

void	free_strarray(char ***array);

void	free_strarrayarray(char ****array);

void	pipe_file_input(char **cmds, char *input_file, int output_fd);

void	pipe_file_output_append(char **cmds, int input_fd, char *output_file);

void	pipe_file_output_trunc(char **cmds, int input_fd, char *output_file);

void	pipe_command(char **cmds, int input_fd, int output_fd);

#endif