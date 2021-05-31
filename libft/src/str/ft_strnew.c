/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 17:17:28 by mjalenqu          #+#    #+#             */
/*   Updated: 2021/04/30 17:09:04 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "ft_str.h"

char	*ft_strnew(int size)
{
	char	*str;

	if (!(str = ft_malloc(size + 1)))
		return (NULL);
	ft_bzero(str, size);
	str[size] = '\0';
	return (str);
}
