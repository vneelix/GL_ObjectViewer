/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 18:18:41 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/08 20:04:12 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilib.h"

void	*ft_memset(void *ptr, int c, size_t n)
{
	void			*ret;
	unsigned long	block;

	ret = ptr;
	block = (unsigned long)c | (unsigned long)c << 8
		| (unsigned long)c << 16 | (unsigned long)c << 24
		| (unsigned long)c << 32 | (unsigned long)c << 40
		| (unsigned long)c << 48 | (unsigned long)c << 56;
	while (n / sizeof(unsigned long) != 0)
	{
		*(unsigned long *)ptr = block;
		ptr += sizeof(unsigned long);
		n -= sizeof(unsigned long);
	}
	while (n != 0)
	{
		*(unsigned char *)ptr++ = c;
		n--;
	}
	return (ret);
}
