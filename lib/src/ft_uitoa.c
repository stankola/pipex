/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 21:12:56 by tsankola          #+#    #+#             */
/*   Updated: 2022/12/09 21:13:56 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

static unsigned int	ft_power(int a, unsigned int power)
{
	unsigned int	result;

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

char	*ft_uitoa(unsigned int n)
{
	int				order_of_magnitude;
	unsigned int	i;
	char			*a;

	order_of_magnitude = 1;
	while (n / ft_power(10, order_of_magnitude) != 0 && order_of_magnitude < 10)
		order_of_magnitude++;
	a = malloc(sizeof(char) * (order_of_magnitude + 1));
	if (a == NULL)
		return (NULL);
	i = 0;
	while (order_of_magnitude > 0)
		a[i++] = '0' + (n / ft_power(10, --order_of_magnitude) % 10);
	a[i] = '\0';
	return (a);
}
