/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 15:24:46 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/31 15:57:47 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	unsigned long *ptr;

	ptr = (unsigned long *)s;
	while (1)
	{
		if ((*ptr & (unsigned long)255) == 0)
			return (0);
		if ((*ptr & (unsigned long)255 << 8) == 0)
			return (1);
		if ((*ptr & (unsigned long)255 << 16) == 0)
			return (2);
		if ((*ptr & (unsigned long)255 << 24) == 0)
			return (3);
		if ((*ptr & (unsigned long)255 << 32) == 0)
			return (4);
		if ((*ptr & (unsigned long)255 << 40) == 0)
			return (5);
		if ((*ptr & (unsigned long)255 << 48) == 0)
			return (6);
		if ((*ptr & (unsigned long)255 << 56) == 0)
			return (7);
	}
	return (0);
}

int	main(void) {
	char *s = calloc(32, 1);
	s[0] = '1';
	s[1] = '2';
	s[2] = '3';
	size_t l = ft_strlen(s);
	return (0);
}
