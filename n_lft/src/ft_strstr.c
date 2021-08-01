/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/06 23:10:25 by vneelix           #+#    #+#             */
/*   Updated: 2019/09/09 17:12:25 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	len[2];

	len[0] = ft_strlen(haystack);
	len[1] = ft_strlen(needle);
	if (len[1] > len[0])
		return (NULL);
	i = 0;
	while (haystack[i] != '\0' && (len[0] - i >= len[1]))
	{
		if (ft_strncmp(haystack + i, needle, len[1]) == 0)
			return ((char*)(haystack + i));
		i += 1;
	}
	return (NULL);
}
