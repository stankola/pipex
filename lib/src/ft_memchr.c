/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:08:11 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/07 18:16:13 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned int	i;
	unsigned char	*sp;

	i = 0;
	sp = (unsigned char *)s;
	while (i < n)
	{
		if (sp[i] == (unsigned char)c)
			return ((void *)&(((unsigned char *)s)[i]));
		i++;
	}
	return (NULL);
}
