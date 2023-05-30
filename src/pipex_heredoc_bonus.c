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
#include "libft.h"
#include "pipex_heredoc_bonus.h"

void	print_pipes(int count)
{
	while (count-- > 0)
		ft_printf("pipe ");
	ft_printf("heredoc>");
}

int	check_limiter(t_char_buffer *buf, char *limiter)
{
	int	length;
	int	i;

	if (get_last_char(buf) != '\n' || get_last_char(buf) == '\0')
		return (0);
	if (limiter != NULL)
	{
		length = ft_strlen(limiter);
		i = 1;
		while (i <= length
			&& (get_char_from_tail(buf, i) != '\n'
				|| get_char_from_tail(buf, i) != '\0'))
		{
			if (limiter[length - i] == get_char_from_tail(buf, i))
				i++;
			else
				return (0);
		}
		if (get_char_from_tail(buf, i) == '\0'
			|| get_char_from_tail(buf, i) == '\n')
			return (1);
	}
	else if (get_char_from_tail(buf, 1) == '\n')
		return (1);
	return (0);
}

// Might be problems here, if the data contains '\0' signs
void	heredoc_reader(char *limiter, int output, int pipecount)
{
	t_char_buffer	*buf;

	buf = new_char_buffer();
	while (! check_limiter(buf, limiter))
	{
		if (get_last_char(buf) == '\0' || get_last_char(buf) == '\n')
			print_pipes(pipecount);
		read_char_to_buffer(buf, STDIN_FILENO);
	}
	if (limiter != NULL)
		erase_chars(buf, ft_strlen(limiter) + 1);
	else
		erase_chars(buf, 1);
	write(output, buf->buffer, buf->tail_index);
	free(buf->buffer);
	free(buf);
	return ;
}
