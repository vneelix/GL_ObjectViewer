/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 13:05:46 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 17:03:11 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilib.h"

char	*ft_strdup(const char *s)
{
	size_t		l;
	const char	*dup;

	if (s == NULL)
		return (NULL);
	l = ft_strlen(s);
	dup = malloc(l + 1);
	if (dup == NULL)
		return (NULL);
	return (ft_memcpy((void *)dup, (void *)s, l + 1));
}
