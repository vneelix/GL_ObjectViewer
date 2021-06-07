/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/06 21:36:41 by vneelix           #+#    #+#             */
/*   Updated: 2020/07/15 15:32:55 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	size_t	len[2];

	if (!dest || !src)
		return (NULL);
	len[0] = ft_strlen(dest);
	len[1] = ft_strlen(src);
	ft_memcpy(dest + len[0], src, len[1]);
	dest[len[0] + len[1]] = '\0';
	return (dest);
}
