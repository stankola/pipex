/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:03:07 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/09 12:03:08 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		length;
	char	*s2;

	length = ft_strlen(s1);
	s2 = malloc(sizeof(char) * (length + 1));
	if (s2 == NULL)
		return (NULL);
	ft_strlcpy(s2, s1, length + 1);
	return (s2);
}
