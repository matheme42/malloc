/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 12:54:53 by matheme           #+#    #+#             */
/*   Updated: 2021/05/31 17:26:55 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc_function.h"

static void *try_local_realloc(t_malloc_header *header, t_malloc_header *next_header, size_t realloc_size)
{
	size_t			need_size;
	t_malloc_header *tmp_header;

	need_size = realloc_size - (header->size & SIZE);
	if ((next_header->size & LOCK) != 0 || (next_header->size & SIZE) < need_size)
		return (NULL);
	next_header->size -= need_size;
	header->size += need_size;
	if ((next_header->size & SIZE) == 0)
	{
		header->size += sizeof(*next_header);
		header->size |= (next_header->size & NEXT);
		header->next = next_header->next;
		ft_bzero(next_header, sizeof(*next_header));
		return ((void*)header + sizeof(*header));
	}
	tmp_header = next_header;
	next_header = (void*)header + sizeof(*header) + (header->size & SIZE);
	next_header->next = tmp_header->next;
	next_header->size = tmp_header->size;
	return ((void*)header + sizeof(*header));
}

static void *local_realloc_fail(t_malloc_header *ptr, size_t realloc_size)
{
	void *rtn;

	rtn = malloc(realloc_size);
	ft_memcpy(rtn, (void*)ptr + sizeof(*ptr), (ptr->size & SIZE));
	free(ptr);
	return (rtn);
}

static void *realloc_tiny_small(void *ptr, t_malloc_header *header, size_t realloc_size, size_t memory_size)
{
	size_t			size;
	t_malloc_header *next_header;
	void			*rtn;

	size = 0;
	if (!header)
		return (0);

    while (size < memory_size && (void*)header + sizeof(*header) != ptr)
    {
		size += sizeof(*header) + (header->size & SIZE);
        if (header->size & NEXT)
		{
			header = header->next;
			size = 0;
		}
		else if (size < memory_size)
			header = (void*)header + sizeof(*header) + (header->size & SIZE);
	}
	if ((void*)header + sizeof(*header) == ptr && header->size & LOCK)
	{
		if ((header->size & SIZE) >= realloc_size)
			return ((void*)header + sizeof(*header));
		next_header = (void*)header + sizeof(*header) + (header->size & SIZE);
		if ((rtn = try_local_realloc(header, next_header, realloc_size)) != NULL)
			return (rtn);
		return (local_realloc_fail(ptr, realloc_size));
	}
	return (NULL);
}

static void *realloc_large(void *ptr, t_malloc_header *header, size_t realloc_size, size_t pagesize)
{
	size_t	need_size;
	size_t	new_realloc_size;
	void	*rtn;

	while ((void*)header + sizeof(*header) != ptr && header->next)
		header = header->next;
	if ((void*)header + sizeof(*header) != ptr)
		return (NULL);
	need_size = realloc_size - header->size;
	new_realloc_size = pagesize;
	if (need_size <= 0)
		return (ptr);
	while (new_realloc_size < need_size)
		new_realloc_size += pagesize;
	
	if (mdalloc((void*)header + header->size + sizeof(*header), new_realloc_size))
	{
		header->size += new_realloc_size;
		return (ptr);
	}
	if ((rtn = malloc(realloc_size)))
	{
		ft_memcpy(rtn, ptr, header->size);
		free(ptr);
		return (rtn);
	}
	return (NULL);
}

void		*realloc(void *ptr, size_t size)
{
	void *rtn;

	// Rend toutes les allocations modulo % 16 pour l'alignement memoire
	while (size % 16)
		size++;

	if ((rtn = realloc_tiny_small(ptr, g_ptr.tiny_ptr, size, g_ptr.tiny_area_size)) != NULL)
		return (rtn);
	if ((rtn = realloc_tiny_small(ptr, g_ptr.small_ptr, size, g_ptr.small_area_size)) != NULL)
		return (rtn);
	return (realloc_large(ptr, g_ptr.large_ptr, size, getpagesize()));
}
