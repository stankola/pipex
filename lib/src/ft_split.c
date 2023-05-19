/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:03:49 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/09 12:03:50 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

static unsigned int	countwords(char const *s, char delimiter)
{
	unsigned int	count;
	char			prev;

	count = 0;
	prev = delimiter;
	while (*s != '\0')
	{
		if (*s != delimiter && prev == delimiter)
			count++;
		prev = *s;
		s++;
	}
	return (count);
}

static unsigned int	countchars(char const *s, char c)
{
	unsigned int	count;

	count = 0;
	while (*s != c && *s != '\0')
	{
		count++;
		s++;
	}
	return (count);
}

static void	*freearray(char **array, unsigned int length)
{
	unsigned int	i;

	i = 0;
	while (i < length)
	{
		if (array[i] != NULL)
			free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	unsigned int	i;
	unsigned int	wordcount;
	unsigned int	charcount;
	char			**wordarray;

	if (s == NULL)
		return (NULL);
	wordcount = countwords(s, c);
	wordarray = ft_calloc(sizeof(char *), wordcount + 1);
	if (wordarray == NULL)
		return (NULL);
	i = 0;
	while (i < wordcount)
	{
		if (*s++ == c)
			continue ;
		charcount = countchars(--s, c);
		wordarray[i] = malloc(sizeof(char) * (charcount + 1));
		if (wordarray[i] == NULL)
			return (freearray(wordarray, i));
		ft_strlcpy(wordarray[i], s, charcount + 1);
		s += charcount + 1;
		i++;
	}
	return (wordarray);
}
