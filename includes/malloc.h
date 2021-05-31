/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 12:53:17 by matheme           #+#    #+#             */
/*   Updated: 2021/04/30 16:11:04 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

#include <machine/_types.h> /* __darwin_size_t */
typedef __darwin_size_t        size_t;

void							free(void *ptr);
void							*malloc(size_t size);
void							*realloc(void *ptr, size_t size);
void							show_alloc_mem();

#endif