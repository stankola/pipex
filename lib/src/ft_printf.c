/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 19:29:51 by tsankola          #+#    #+#             */
/*   Updated: 2022/12/09 19:29:52 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "libft.h"
#ifndef FORMAT_CHARACTERS
# define FORMAT_CHARACTERS "scdiuxXp%"
#endif
#ifndef NUMBER_CHARACTERS
# define NUMBER_CHARACTERS "diuxXp"
#endif

/*
 * According to SO/IEC 9899:201x:
 * If a conversion specification is invalid, the behavior is undefined.282) If
 * any argument is not the correct type for the corresponding conversion
 * specification, the behavior is undefined.
 *
 * If this function were to implement printf fully, then all these conversions
 * should be implemented:
 * "-+ #0123456789diouxXaAeEfFgGhljztLcspn*,.;:_"
 * AND hh, ll, vh, hv, vl, lv, vll, llv (? I think...)
 */

static int	write_number(va_list iterator, char type, int fd)
{
	char	*s;
	char	*s1;
	int		print_count;

	s = NULL;
	if (type == 'd' || type == 'i')
		s = ft_itoa(va_arg(iterator, int));
	else if (type == 'u')
		s = ft_uitoa(va_arg(iterator, int));
	else if (type == 'x' || type == 'X')
		s = ft_itoa16(va_arg(iterator, int), type == 'X');
	else if (type == 'p')
	{	
		s1 = ft_ulitoa16(va_arg(iterator, unsigned long int), 0);
		s = ft_strjoin("0x", s1);
		free(s1);
	}
	print_count = write(fd, s, ft_strlen(s));
	free(s);
	return (print_count);
}

static int	print_argument(va_list iterator, char type, int fd)
{
	char	*s;

	s = NULL;
	if (type == 's')
	{
		s = va_arg(iterator, char *);
		if (s == NULL)
			return (write(fd, "(null)", 6));
		else
			return (write(fd, s, ft_strlen(s)));
	}
	else if (type == 'c')
		return (ft_putchar_fd((char)va_arg(iterator, int), fd));
	else if (ft_strchr(NUMBER_CHARACTERS, type))
		return (write_number(iterator, type, fd));
	else if (type == '%')
		return (ft_putchar_fd('%', fd));
	return (0);
}

static int	print_string(const char *s, va_list iterator, int fd)
{
	int		format_specifier_found;
	int		print_count;

	format_specifier_found = 0;
	print_count = 0;
	while (*s != '\0')
	{
		if (!format_specifier_found && *s != '%')
			print_count += ft_putchar_fd(*s++, fd);
		else if (format_specifier_found)
		{
			if (ft_strchr(FORMAT_CHARACTERS, *s++))
				print_count += print_argument(iterator, *(s - 1), fd);
			format_specifier_found = 0;
		}
		else if (*s++ == '%')
			format_specifier_found = 1;
	}
	return (print_count);
}

int	ft_fprintf(int fd, const char *s, ...)
{
	int		print_count;
	va_list	iterator;

	if (s == NULL)
		return (-1);
	va_start(iterator, s);
	print_count = print_string(s, iterator, fd);
	va_end(iterator);
	return (print_count);
}

int	ft_printf(const char *s, ...)
{
	int		print_count;
	va_list	iterator;

	if (s == NULL)
		return (-1);
	va_start(iterator, s);
	print_count = print_string(s, iterator, STDOUT_FILENO);
	va_end(iterator);
	return (print_count);
}
