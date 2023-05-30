/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 15:54:10 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/27 15:54:11 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_HEREDOC_BONUS_H
# define PIPEX_HEREDOC_BONUS_H
# include <stdlib.h>

typedef struct s_char_buffer
{
	char	*buffer;
	size_t	size;
	size_t	tail_index;
}				t_char_buffer;

void			heredoc_reader(char *limiter, int output, int pipecount);

t_char_buffer	*new_char_buffer(void);

int				read_char_to_buffer(t_char_buffer *buf, int fd);

char			get_last_char(t_char_buffer *buf);

void			erase_chars(t_char_buffer *buf, size_t count);

char			get_char_from_tail(t_char_buffer *buf, unsigned int offset);

#endif
