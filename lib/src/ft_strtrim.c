/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 19:12:40 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/07 19:12:41 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

// Empty character is part of all sets.
static int	ft_istrimmable(char c, char const *set)
{
	if (c == '\0')
		return (1);
	while (*set != '\0')
	{
		if (c == *(set++))
			return (1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	trimmedlength;
	char			*trim_string;

	if (set == NULL)
		return (ft_strdup(s1));
	if (s1 == NULL)
		return (NULL);
	while (*s1 != '\0' && ft_istrimmable(*s1, set))
		s1++;
	trimmedlength = ft_strlen(s1);
	if (trimmedlength > 0)
		while (ft_istrimmable(*(s1 + trimmedlength - 1), set))
			trimmedlength--;
	trim_string = ft_substr(s1, 0, trimmedlength);
	return (trim_string);
}
