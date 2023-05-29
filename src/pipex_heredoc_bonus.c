/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 15:35:45 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/27 15:35:45 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "pipex.h"
#include "libft.h"
#include "pipex_heredoc_bonus.h"
#ifndef PIPEX_READ_BUFFER_SIZE
# define PIPEX_READ_BUFFER_SIZE 1
#endif

static t_char_buffer	*new_char_buffer(void)
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

static int	read_char_to_buffer(t_char_buffer *buf, int fd)
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

static char	get_last_char(t_char_buffer *buf)
{
	if (buf->tail_index > 0)
		return (buf->buffer[buf->tail_index - 1]);
	return ('\0');
}

static void	erase_chars(t_char_buffer *buf, size_t count)
{
	if (count > buf->tail_index)
		buf->tail_index = 0;
	else
		buf->tail_index -= count;
	buf->buffer[buf->tail_index] = '\0';
}

void	heredoc_reader(char *limiter, int output, int pipecount)
{
	char			*limit_check;
	t_char_buffer	*buf;
	int				start_checking;

	pipecount += 0;
	buf = new_char_buffer();
	limit_check = limiter;
	start_checking = 1;
	while ((limit_check == NULL || *limit_check != '\0') && read_char_to_buffer(buf, STDIN_FILENO) > 0)
	{
		if (start_checking)
		{
			if (limit_check == NULL &&  get_last_char(buf) == '\n')
			{
				erase_chars(buf, 1);
				break ;
			}
			else if (limit_check != NULL && get_last_char(buf) == *(limit_check))
				limit_check++;
			else
			{
				limit_check = limiter;
				start_checking = 0;
			}
		}
		start_checking = start_checking || get_last_char(buf) == '\n';
	}
	if (limit_check != NULL && *limit_check == '\0')
		erase_chars(buf, ft_strlen(limiter));
	write(output, buf->buffer, buf->tail_index);
	free(buf->buffer);
	free(buf);
	return ;
}
