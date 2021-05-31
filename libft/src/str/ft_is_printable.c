/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_printable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicaster <vicaster@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 13:12:23 by mjalenqu          #+#    #+#             */
/*   Updated: 2020/11/25 10:53:09 by vicaster         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/ft_str.h"

int		ft_is_printable(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}
