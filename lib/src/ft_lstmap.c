/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:04:48 by tsankola          #+#    #+#             */
/*   Updated: 2022/11/07 18:07:03 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

/*
Iterates the list ’lst’ and applies the function
’f’ on the content of each node. Creates a new
list resulting of the successive applications of
the function ’f’. The ’del’ function is used to
delete the content of a node if needed.
*/

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	void	*new_content;

	new_content = NULL;
	if (lst == NULL || (*f) == NULL || (*del) == NULL)
		return (NULL);
	new_content = (*f)(lst->content);
	new_node = ft_lstnew(new_content);
	if (new_node == NULL)
	{
		(*del)(new_content);
		return (NULL);
	}
	if (lst->next != NULL)
	{
		new_node->next = ft_lstmap(lst->next, (*f), (*del));
		if (new_node->next == NULL)
		{
			(*del)(new_node->content);
			free(new_node);
			return (NULL);
		}
	}
	return (new_node);
}
