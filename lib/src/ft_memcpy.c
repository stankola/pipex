/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 20:06:31 by tsankola          #+#    #+#             */
/*   Updated: 2022/10/29 20:18:18 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned int	count;

	count = 0;
	if ((dst == NULL && src == NULL) && n > 0)
		return (NULL);
	while (count < n)
	{
		*((char *)dst + count) = *((const char *)src + count);
		count++;
	}
	return (dst);
}
