// TODO header

#ifndef PIPEX_H
# define PIPEX_H
# ifndef CHILD_END
#  define CHILD_END 1
# endif
# ifndef PARENT_END
#  define PARENT_END 0
# endif

char	*get_errmsg();

char	**get_env_path_value();

char	*find_cmd(char *exe);

char	*get_full_path(char const *path, char const *file);

int		check_file_execution_access(char *location);

int		is_directory(char *file);

#endif