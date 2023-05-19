/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 20:20:04 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/07 20:24:27 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

/*
Applies the function ’f’ to each character of the string ’s’, and passing
its index as first argument to create a new string (with malloc(3)) resulting
from successive applications of ’f’.
*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*map;
	unsigned int	len;

	if (f == NULL)
		return (NULL);
	if (s == NULL)
		return (NULL);
	i = 0;
	len = (unsigned int)ft_strlen(s);
	map = malloc(sizeof(char) * (len + 1));
	if (map == NULL)
		return (NULL);
	while (i < len)
	{
		map[i] = (*f)(i, s[i]);
		i++;
	}
	map[i] = '\0';
	return (map);
}
