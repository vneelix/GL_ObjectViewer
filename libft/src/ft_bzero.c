/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 12:53:47 by vneelix           #+#    #+#             */
/*   Updated: 2021/05/16 16:19:59 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *nptr, size_t n)
{
	while (n / sizeof(uint64_t))
	{
		*((uint64_t *)nptr) = 0;
		nptr = (uint64_t *)nptr + 1;
		n -= sizeof(uint64_t);
	}
	while (n % sizeof(uint64_t))
	{
		*((uint8_t *)nptr++) = 0;
		n -= sizeof(uint8_t);
	}
}
