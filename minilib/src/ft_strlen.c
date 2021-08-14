/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 13:06:32 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 13:08:05 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilib.h"

size_t	ft_strlen(const char *s)
{
	const char	*p;

	if (s == NULL)
		return (0);
	p = s;
	while (*p != '\0')
		p++;
	return ((size_t)(p - s));
}
