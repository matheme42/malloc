/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:59:51 by matheme           #+#    #+#             */
/*   Updated: 2021/05/27 17:19:10 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc_function.h"

static char name[2][5] = {"LOCK\0", "FREE\0"};

int	ft_putnbrbase(unsigned long long adr, int bas)
{
	static char *base = "0123456789ABCDEF\0";

	if (adr / bas > 0)
		return (ft_putnbrbase(adr / bas, bas) + ft_putchar(base[adr % bas]));
	return (ft_putchar(base[adr % bas]));
}

void print_title(char *title, unsigned int number)
{
	ft_putstr(title);
	ft_putnbrbase(number, 16);
	ft_putchar('\n');
}

void print_addr(unsigned int addr1, unsigned int addr2, unsigned int number)
{
	addr1 = (unsigned int)addr1;
	addr2 = (unsigned int)addr2;
	number = (unsigned int)number;

	ft_putnbrbase(addr1, 16);
	ft_putstr(" - ");
	ft_putnbrbase(addr2, 16);
	ft_putchar(' ');
	ft_putnbrbase(number, 10);
	ft_putstr(" octects");
}

static size_t		show_tiny()
{
	size_t			size;
	size_t			gsize;
	t_malloc_header *header;

	size = 0;
	gsize = 0;
	header = g_ptr.tiny_ptr;
	if (!header)
		return (0);
	print_title("TINY: ", (unsigned int)header);
	while (gsize < g_ptr.tiny_area_size)
	{
		size += (header->size & LOCK) ? header->size & SIZE : 0;
		print_addr((unsigned int)((void*)header + sizeof(*header)), (unsigned int)((void*)header + sizeof(*header) + (header->size & SIZE)), (header->size & SIZE));
		dprintf(1, "  (%s)\n", name[(header->size & LOCK) == 0]);
		gsize += (header->size & SIZE) + sizeof(*header);
		if (header->size & NEXT)
		{
			gsize = 0;
			header = header->next;
		}
		else if (gsize < g_ptr.tiny_area_size)
			header = (void*)header + (header->size & SIZE) + sizeof(*header);
	}
	return (size);
}

static unsigned int show_small()
{
	unsigned int	size;
	size_t			gsize;
	t_malloc_header	*header;

	size = 0;
	gsize = 0;
	header = g_ptr.small_ptr;
	if (!header)
		return (0);
	print_title("SMALL: ", (unsigned int)header);
	while (gsize < g_ptr.small_area_size)
	{
		if (header->size & NEXT)
		{
			gsize = 0;
			header = header->next;
		}
		else
		{
			size += (header->size & LOCK) ? header->size & SIZE : 0;
			if ((header->size & LOCK))
				print_addr((unsigned int)((void*)header + sizeof(*header)), (unsigned int)((void*)header + sizeof(*header) + (header->size & SIZE)), (header->size & SIZE));
			gsize += (header->size & SIZE) + sizeof(*header);
			if (gsize < g_ptr.small_area_size)
				header = (void*)header + (header->size & SIZE) + sizeof(*header);
		}
	}
	return (size);
}

static unsigned int show_large()
{
	unsigned int			size;
	t_malloc_header	*header;

	size = 0;
	header = g_ptr.large_ptr;
	if (!header)
		return (0);
	print_title("LARGE: ", (unsigned int)header);
	while (header)
	{
			print_addr((unsigned int)((void*)header + sizeof(*header)), (unsigned int)((void*)header + sizeof(*header) + (header->size & SIZE)), (header->size & SIZE));
			ft_putstr("\n");
			size += header->size & SIZE;
			header = header->next;
	}
	return (size);
}

void	show_alloc_mem()
{
	unsigned int	size;

	size = 0;
	size += show_tiny();
	size += show_small();
	size += show_large();

	ft_putstr("Total : ");
	ft_putnbrbase(size, 10);
	ft_putstr(" octects\n");
}
