/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/06 21:54:55 by vneelix           #+#    #+#             */
/*   Updated: 2019/09/08 23:42:26 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *dest, const char *src, size_t n)
{
	size_t	len[2];

	len[0] = ft_strlen(dest);
	len[1] = ft_strlen(src);
	len[1] = (len[1] > n ? n : len[1]);
	ft_memcpy(dest + len[0], src, len[1]);
	dest[len[0] + len[1]] = '\0';
	return (dest);
}
