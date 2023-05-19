/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 20:15:49 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/07 20:16:14 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char				*sub;
	unsigned int		s_len;
	unsigned int		sub_len;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		sub_len = 1;
	else if (s_len - start < len)
		sub_len = s_len - start + 1;
	else
		sub_len = len + 1;
	sub = malloc(sub_len * sizeof(char));
	if (sub == NULL)
		return (NULL);
	if (sub_len == 1)
		*sub = '\0';
	else
		ft_strlcpy(sub, s + start, sub_len);
	return (sub);
}
