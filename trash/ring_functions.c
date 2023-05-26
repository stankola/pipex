/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:22:57 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/10 13:22:59 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ring.h"
#include <stdlib.h>
#include <unistd.h>

int	ring_get_size(t_ring *ring)
{
	t_ring	*temp_ring;
	int		i;

	if (ring == NULL)
		return (0);
	temp_ring = ring->next;
	i = 1;
	while (temp_ring != ring)
	{
		i++;
		temp_ring = temp_ring->next;
	}
	return (i);
}

t_ring	*ring_clone(t_ring *ring)
{
	t_ring	*clone;
	t_ring	*iterator;

	if (ring == NULL)
		return (NULL);
	iterator = ring->next;
	clone = NULL;
	while (iterator != ring)
	{
		ring_add(&clone, iterator->content);
		iterator = iterator->next;
	}
	ring_add(&clone, iterator->content);
	return (clone);
}

int	ring_is_equal(t_ring *ring_a, t_ring *ring_b, int (*f)(void *, void *))
{
	t_ring	*a_iterator;
	t_ring	*b_iterator;

	if (ring_a == NULL && ring_b == NULL)
		return (1);
	if (ring_a == NULL || ring_b == NULL || f == NULL)
		return (0);
	a_iterator = ring_a->prev;
	b_iterator = ring_b->prev;
	while (a_iterator != ring_a || b_iterator != ring_b)
	{
		if (! f(a_iterator->content, b_iterator->content))
			return (0);
		a_iterator = a_iterator->prev;
		b_iterator = b_iterator->prev;
	}
	if (! f(a_iterator->content, b_iterator->content))
		return (0);
	return (1);
}

// searches the ring for the item that f(item, target) returns nonzero and
// rotates it to the front.
int	ring_find(t_ring **ring, void *target, int (*f)(void *, void *))
{
	t_ring	*start;

	start = *ring;
	while (1)
	{
		if (f((*ring)->content, target))
			return (1);
		else
			ring_rotate(ring);
		if (start != *ring)
			break;
	}
	return (0);
}
