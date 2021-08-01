/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 19:57:20 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/02 10:26:37 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		((__uint8_t*)dest)[i] = ((__uint8_t*)src)[i];
		if (((__uint8_t*)dest)[i] == (__uint8_t)(c))
			return (dest + i);
		i += 1;
	}
	return (NULL);
}
