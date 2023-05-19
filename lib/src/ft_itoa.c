/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 17:56:58 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/07 18:01:26 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

static int	ft_pow(int a, unsigned int power)
{
	int	result;

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

char	*ft_itoa(int n)
{
	int				order_of_magnitude;
	int				negative;
	unsigned int	i;
	char			*a;

	order_of_magnitude = 1;
	if (n < 0)
		negative = -1;
	else
		negative = 0;
	while (n / ft_pow(10, order_of_magnitude) != 0 && order_of_magnitude < 10)
		order_of_magnitude++;
	a = malloc(sizeof(char) * (order_of_magnitude - negative + 1));
	if (a == NULL)
		return (NULL);
	i = 0;
	if (negative < 0)
		a[i++] = '-';
	else
		negative = 1;
	while (order_of_magnitude > 0)
		a[i++] = '0' + (negative * (n / ft_pow(10, --order_of_magnitude) % 10));
	a[i] = '\0';
	return (a);
}
