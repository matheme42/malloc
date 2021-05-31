/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 12:54:30 by matheme           #+#    #+#             */
/*   Updated: 2021/05/27 17:27:15 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc_function.h"

static void	defragment_memory(t_malloc_header *header, t_malloc_header *old_header, size_t size, size_t memory_size)
{
	t_malloc_header *next;

	next = NULL;

	if (size + sizeof(*header) + (header->size & SIZE) <= memory_size)
		next = (void*)header + sizeof(*header) + (header->size & SIZE);
	if (next && (next->size & LOCK) == 0)
	{
		header->size += sizeof(*header) + next->size & SIZE;
		header->size |= (next->size & NEXT);
		header->next = next->next;
	}
	if (old_header && (old_header->size & LOCK) == 0)
	{
		old_header->size += sizeof(*header) + header->size & SIZE;
		old_header->size |= (header->size & NEXT);
		old_header->next = header->next;
	}
}

static void	free_page(t_malloc_header *header, size_t memory_size, t_malloc_header *old_page)
{
	void *next;

	next = header->next;
	if ((header->size & SIZE) == memory_size && old_page)
	{
		munmap((void*)header, memory_size);
		old_page->next = next;
		old_page->size = old_page->size & ~NEXT;
	}
	else if (next && !old_page && (header->size & SIZE) == memory_size)
	{
		if (header == g_ptr.tiny_ptr)
			g_ptr.tiny_ptr = next;
		else if (header == g_ptr.small_ptr)
			g_ptr.small_ptr = next;
	}
}

static void free_large(void *ptr)
{
	t_malloc_header *header;
	t_malloc_header *oldheader;

	header = g_ptr.large_ptr;
	oldheader = g_ptr.large_ptr;
	if (!header)
		return ;
	while (header != NULL && (void*)header + sizeof(*header) != ptr)
	{
		oldheader = header;
		header = header->next;
	}
	if ((void*)header + sizeof(*header) == ptr)
	{
		if (header == oldheader)
			g_ptr.large_ptr = header->next;
		else
			oldheader->next = header->next;
		munmap(header, header->size + sizeof(*header));
	}
}

static int free_tiny_small(void *ptr, t_malloc_header *header, size_t memory_size)
{
	size_t			size;
	t_malloc_header	*old_header;
	t_malloc_header	*oldpage;

	size = 0;
	if (!header)
		return (0);
	old_header = NULL;
	oldpage = NULL;
    while (size < memory_size && (void*)header + sizeof(*header) != ptr)
    {
		size += sizeof(*header) + (header->size & SIZE);
        if (header->size & NEXT)
		{
			oldpage = header;
			header = header->next;
			size = 0;
			old_header = NULL;
		}
		else if (size < memory_size)
		{
			old_header = header;
			header = (void*)header + sizeof(*header) + (header->size & SIZE);
		}
	}
	if ((void*)header + sizeof(*header) == ptr)
	{
		header->size = header->size & ~LOCK;
		defragment_memory(header, old_header, size, memory_size);
		header = (old_header == NULL) ? header : old_header;
		free_page(header, memory_size, oldpage);
		return (1);
	}
	return (0);
}

void free(void *ptr)
{
	if (free_tiny_small(ptr, g_ptr.tiny_ptr, g_ptr.tiny_area_size))
		return ;
	if (free_tiny_small(ptr, g_ptr.small_ptr, g_ptr.small_area_size))
		return ;
	free_large(ptr);
}
