/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ulitoa16.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 21:24:33 by tsankola          #+#    #+#             */
/*   Updated: 2022/12/09 21:25:07 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static unsigned long int	ft_power(int a, unsigned int power)
{
	unsigned long int	result;

	if (power == 0)
		return (1);
	result = a;
	while (power > 1)
	{
		result *= a;
		power--;
	}
	return (result);
}

static char	get_a_hex(unsigned int n, int cap)
{
	char	c;

	if (n < 10)
		return ('0' + n);
	else if (n < 16)
		c = 'a' + (n - 10);
	else
		c = 0;
	return (c - (cap * 32));
}

char	*ft_ulitoa16(unsigned long int n, int cap)
{
	int				oom;
	unsigned int	i;
	char			*a;

	oom = 1;
	while (oom < 16 && n / ft_power(16, oom) != 0)
		oom++;
	a = malloc(sizeof(char) * (oom + 1));
	if (a == NULL)
		return (NULL);
	i = 0;
	while (oom > 0)
		a[i++] = get_a_hex((n / ft_power(16, --oom) % 16), cap);
	a[i] = '\0';
	return (a);
}
