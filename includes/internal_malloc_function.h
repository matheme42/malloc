/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_malloc_function.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 13:28:34 by matheme           #+#    #+#             */
/*   Updated: 2021/05/31 17:27:51 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_FUNCTION_MALLOC_H
# define INTERNAL_FUNCTION_MALLOC_H

#include "malloc.h"

// include for munmap & mmap
#include <sys/mman.h>

#include "ft_str.h"

#include <stdio.h>


//		NL-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// size 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
// if the bit N is set the address of the next page is set
// if the bit L is set the malloc is LOCK

#define NEXT 0x8000000000000000 // MASK to set the bit N
#define LOCK 0x4000000000000000 // MASK to set the bit L
#define SIZE (~NEXT & ~LOCK)    // MASK to see the rest of the number

typedef struct					s_malloc_header
{
				size_t			size;
				void			*next;
}								t_malloc_header;

typedef struct					s_ptr
{
								void*	tiny_ptr;
								size_t	tiny_area_size;
								void*	small_ptr;
								size_t	small_area_size;
								void*	large_ptr;
}								t_ptr;

extern							t_ptr g_ptr;

# define MALLOC_MAX_SIZE_TINY_AREA		256
# define MALLOC_MAX_SIZE_SMALL_AREA		1024
# define MALLOC_PER_AREA				100

# define MIN_TINY_AREA_SIZE_PAGE	(MALLOC_MAX_SIZE_TINY_AREA + sizeof(t_malloc_header)) * MALLOC_PER_AREA
# define MIN_SMALL_AREA_SIZE_PAGE	(MALLOC_MAX_SIZE_SMALL_AREA + sizeof(t_malloc_header)) * MALLOC_PER_AREA

void *mdalloc(void *ptr, size_t area_size);

#endif