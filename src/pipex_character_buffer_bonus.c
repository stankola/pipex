/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_character_buffer_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 20:26:15 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/30 20:26:16 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"
#include "pipex_heredoc_bonus.h"
#ifndef PIPEX_READ_BUFFER_SIZE
# define PIPEX_READ_BUFFER_SIZE 1
#endif

t_char_buffer	*new_char_buffer(void)
{
	t_char_buffer	*new_buffer;

	new_buffer = malloc(sizeof(t_char_buffer));
	if (new_buffer == NULL)
		return (NULL);
	new_buffer->buffer = malloc (sizeof(char) * (PIPEX_READ_BUFFER_SIZE + 1));
	if (new_buffer->buffer == NULL)
	{
		free(new_buffer);
		return (NULL);
	}
	new_buffer->size = PIPEX_READ_BUFFER_SIZE;
	new_buffer->tail_index = 0;
	new_buffer->buffer[new_buffer->tail_index] = '\0';
	return (new_buffer);
}

int	read_char_to_buffer(t_char_buffer *buf, int fd)
{
	char	*temp_buf;
	int		read_count;

	if (buf->tail_index + 1 == buf->size)
	{
		temp_buf = malloc(sizeof(char) * (buf->size * 2));
		if (temp_buf == NULL)
		{
			perror(NULL);
			return (-1);
		}
		ft_strlcpy(temp_buf, buf->buffer, buf->size * 2);
		free(buf->buffer);
		buf->buffer = temp_buf;
		buf->size *= 2;
	}
	read_count = read(fd, &buf->buffer[buf->tail_index], 1);
	if (read_count > 0)
	{
		buf->tail_index += read_count;
		buf->buffer[buf->tail_index] = '\0';
	}
	return (read_count);
}

char	get_last_char(t_char_buffer *buf)
{
	if (buf->tail_index > 0)
		return (buf->buffer[buf->tail_index - 1]);
	return ('\0');
}

void	erase_chars(t_char_buffer *buf, size_t count)
{
	if (count > buf->tail_index)
		buf->tail_index = 0;
	else
		buf->tail_index -= count;
	buf->buffer[buf->tail_index] = '\0';
}

char	get_char_from_tail(t_char_buffer *buf, unsigned int offset)
{
	if (offset > buf->tail_index - 1)
		return ('\0');
	return (buf->buffer[buf->tail_index - offset - 1]);
}
