/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:29:44 by tsankola          #+#    #+#             */
/*   Updated: 2023/06/14 14:29:45 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "libft.h"
#include "pipex.h"

static void	insert_string(char ***s_array, char *s)
{
	int		array_size;
	char	**new_array;
	char	**array_i;
	int		i;

	array_size = 0;
	array_i = *s_array;
	while (*(array_i) != NULL)
	{
		array_size++;
		array_i++;
	}
	new_array = ft_calloc(sizeof(char *), array_size + 2);
	if (new_array == NULL)
	{
		perror(NULL);
		exit(errno);
	}
	i = -1;
	while (++i < array_size)
		new_array[i] = (*s_array)[i];
	new_array[i] = s;
	new_array[i + 1] = NULL;
	free(*s_array);
	*s_array = new_array;
}

static char	*get_quoted_word(char *s)
{
	char	*s2;
	int		word_length;

	s2 = ft_strchr(s + 1, *s);
	if (s2 == NULL)
		return (NULL);
	word_length = (s2 - s) / sizeof(char) - 1;
	if (word_length == 0)
		return (NULL);
	s2 = malloc(sizeof(char) * (word_length + 1));
	if (s == NULL)
	{
		perror(NULL);
		exit(errno);
	}
	ft_strlcpy(s2, s + 1, word_length + 1);
	return (s2);
}

// appends length characters to s
static void	append_string(char **s, char *s2, unsigned int length)
{
	int		total_length;
	char	*s3;

	total_length = ft_strlen(*s) + length;
	s3 = malloc(sizeof(char) * (total_length + 1));
	if (s3 == NULL)
	{
		perror(NULL);
		exit(errno);
	}
	ft_strlcpy(s3, *s, total_length + 1);
	ft_strlcat(s3, s2, total_length + 1);
	free(*s);
	*s = s3;
}

static char	*get_word(char **s)
{
	char	*i;
	char	*i2;
	char	*r;

	i = *s;
	r = ft_calloc(sizeof(char), 1);
	while (*i != '\0' && *i != ' ')
	{
		if ((*i == '\'' && ft_strchr(i + 1, '\''))
			|| (*i == '"' && ft_strchr(i + 1, '"')))
		{
			i2 = get_quoted_word(i++);
			if (i2 != NULL)
			{
				append_string(&r, i2, ft_strlen(i2));
				i += ft_strlen(i2);
			}
		}
		else
			append_string(&r, i, 1);
		i++;
	}
	*s = i;
	return (r);
}

char	**get_cmd_and_params(char *s)
{
	char	*i;
	int		word_length;
	char	*i2;
	char	**s_array;

	i = s;
	word_length = 0;
	s_array = ft_calloc(sizeof(char *), 1);
	while (*i != '\0')
	{
		while (ft_isspace(*i))
			i++;
		i2 = get_word(&i);
		insert_string(&s_array, i2);
	}
	return (s_array);
}
