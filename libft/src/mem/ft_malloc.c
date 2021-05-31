/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 15:13:18 by rlegendr          #+#    #+#             */
/*   Updated: 2021/04/30 16:46:14 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "ft_mem.h"

void			*ft_malloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
	{
		ft_printf("memory allocation failed\n");
		ft_printf("program stops\n");
		exit(126);
	}
	ft_memset(ret, '\0', size);
	return (ret);
}
