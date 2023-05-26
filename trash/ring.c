/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:22:16 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/10 13:22:18 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ring.h"
#include <stdlib.h>

// Adds content to the front of the ring or creates a new ring if NULL is passed
// as the first argument
void	ring_add(t_ring **ring, void *content)
{
	t_ring	*newring;

	newring = malloc(sizeof(t_ring));
	if (newring == NULL)
		return ;
	newring->next = newring;
	newring->prev = newring;
	newring->content = content;
	if (*ring != NULL)
	{
		newring->next = (*ring)->next;
		newring->prev = *ring;
		newring->prev->next = newring;
		newring->next->prev = newring;
	}
	*ring = newring;
}

void	*ring_take(t_ring **ring)
{
	void	*content;
	t_ring	*temp_ring;

	if (ring == NULL || *ring == NULL)
		return (NULL);
	temp_ring = (*ring);
	if ((*ring)->next == *ring)
		*ring = NULL;
	else
	{
		*ring = (*ring)->prev;
		(*ring)->next = temp_ring->next;
		(*ring)->next->prev = *ring;
	}
	temp_ring->next = NULL;
	temp_ring->prev = NULL;
	content = temp_ring->content;
	free(temp_ring);
	return (content);
}

void	ring_rotate(t_ring **ring)
{
	if (*ring != NULL)
		*ring = (*ring)->next;
}

void	ring_reverse_rotate(t_ring **ring)
{
	if (*ring != NULL)
		*ring = (*ring)->prev;
}

void	ring_del(t_ring **ring, void (*del)(void*))
{
	void	*content;

	if (*ring == NULL)
		return ;
	content = ring_take(ring);
	if (del != NULL)
		(del)(content);
	if (*ring != NULL)
		ring_del(ring, del);
}
