/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 21:37:53 by vneelix           #+#    #+#             */
/*   Updated: 2020/08/17 20:40:29 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	uint8_t *ptr;

	ptr = (uint8_t*)s;
	while (n)
	{
		if (*ptr == (uint8_t)c)
			return ((void*)ptr);
		ptr += 1;
		n -= 1;
	}
	return (*ptr == (uint8_t)c ? ptr : NULL);
}
