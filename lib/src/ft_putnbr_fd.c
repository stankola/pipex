/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:05:41 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/09 12:05:42 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
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

void	ft_putnbr_fd(int n, int fd)
{
	int			oom;
	int			sign;
	long int	long_n;

	long_n = n;
	oom = 0;
	if (long_n < 0)
	{
		ft_putchar_fd('-', fd);
		sign = -1;
	}
	else
		sign = 1;
	long_n *= sign;
	while ((long_n / ft_pow(10, oom)) > 9)
		oom++;
	while (oom >= 0)
		ft_putchar_fd('0' + ((long_n / ft_pow(10, oom--)) % 10), fd);
}
