/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:04:42 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/07 18:04:43 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if ((*f) == NULL || lst == NULL)
		return ;
	(*f)(lst->content);
	if (lst->next != NULL)
		ft_lstiter(lst->next, (*f));
}
