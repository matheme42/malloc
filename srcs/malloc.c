/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 12:53:56 by matheme           #+#    #+#             */
/*   Updated: 2021/05/31 15:56:58 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc_function.h"

t_ptr	g_ptr;

// direct allocation
void *mdalloc(void *ptr, size_t area_size)
{
	void *p;

	if (ptr)
		p = mmap(ptr, area_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);
	else
		p = mmap(ptr, area_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if (p == (void*)0xffffffffffffffff)
		p = NULL;
	if (p != NULL)
		ft_bzero(p, area_size);
	return (p);
}

static t_malloc_header *add_new_page(t_malloc_header *header, size_t memory_size)
{
    if (header->size & NEXT)
        return (header->next);
    header->size |= NEXT;
    while (header->next == NULL)
        header->next = mdalloc(NULL, memory_size);
    header = header->next;
    header->size = memory_size;
    return (header);
}

static t_malloc_header *find_free_header(t_malloc_header *header, size_t memory_size, size_t malloc_size)
{
	size_t size;

	size = 0;
    while (1)
    {
        if (!(header->size & LOCK) && (header->size & SIZE) >= malloc_size)
        {
			if ((header->size & SIZE) - malloc_size >= sizeof(*header))
			{
				((t_malloc_header*)((void*)header + sizeof(*header) + malloc_size))->size = (header->size & SIZE) - (sizeof(*header) + malloc_size);
				header->size = malloc_size;
			
			}
            header->size |= LOCK;
            return ((void*)header + sizeof(*header));
        }
		size += (header->size & SIZE) + sizeof(*header);
		if (size >= memory_size)
		{
			header = add_new_page(header, memory_size);
			size = 0;
			continue ;
		}	
		header = (void*)header + (header->size & SIZE) + sizeof(*header);
    }
    return (NULL);
}

static void	*malloc_tiny_small(size_t malloc_size, size_t memory_size, t_malloc_header **memory)
{
	if (*memory == NULL)
	{
		*memory = mdalloc(NULL, memory_size);
		(*memory)->size = memory_size;
	}
	return (find_free_header(*memory, memory_size, malloc_size));
}

static void	*malloc_large(size_t size, size_t pagesize)
{
	t_malloc_header			*header;
	size_t					real_size;
	void					*p;

	real_size = pagesize;
	while (real_size < size + sizeof(*header))
		real_size += pagesize;

	p = mdalloc(NULL, size + sizeof(*header));
	if (g_ptr.large_ptr == NULL)
	{
		g_ptr.large_ptr = p;
		header = g_ptr.large_ptr;
	} else
	{
		header = g_ptr.large_ptr;
		while  (header->next != NULL)
			header = header->next;
		header->next = p;
		header = header->next;
	}
	header->size = real_size - sizeof(*header);
	header->next = NULL;
	return ((void*)header + sizeof(*header));
}

static void get_area_size(size_t pagesize)
{
	if (g_ptr.tiny_area_size && g_ptr.small_area_size)
		return ;
	g_ptr.tiny_area_size = pagesize;
	while (g_ptr.tiny_area_size < MIN_TINY_AREA_SIZE_PAGE)
		g_ptr.tiny_area_size += pagesize;
	g_ptr.small_area_size = pagesize;
	while (g_ptr.small_area_size < MIN_SMALL_AREA_SIZE_PAGE)
		g_ptr.small_area_size += pagesize;
}

void	*malloc(size_t size)
{
	get_area_size(getpagesize());

	// Rend toutes les allocations modulo%16 pour l'alignement memoire
	while (size % 16)
		size++;

	if (size <= MALLOC_MAX_SIZE_TINY_AREA)
		return (malloc_tiny_small(size, g_ptr.tiny_area_size, (t_malloc_header **)&(g_ptr.tiny_ptr)));
	else if (size <= MALLOC_MAX_SIZE_SMALL_AREA)
		return (malloc_tiny_small(size, g_ptr.small_area_size, (t_malloc_header **)&(g_ptr.small_ptr)));
	return (malloc_large(size, getpagesize()));
}
