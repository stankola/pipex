/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:02:34 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/09 12:02:35 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		length;
	int		s1_len;
	int		s2_len;
	char	*s3;

	s1_len = 0;
	s2_len = 0;
	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 != NULL)
		s1_len = ft_strlen(s1);
	if (s2 != NULL)
		s2_len = ft_strlen(s2);
	length = s1_len + s2_len;
	s3 = ft_calloc(sizeof(char), (length + 1));
	if (s3 == NULL)
		return (NULL);
	if (s1_len > 0)
		ft_strlcpy(s3, s1, s1_len + 1);
	if (s2_len > 0)
		ft_strlcat(s3, s2, length + 1);
	return (s3);
}
