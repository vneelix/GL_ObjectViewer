/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 20:29:15 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/23 20:29:16 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	len;
	char	*ret;

	len = ft_strlen(s1);
	len = (n > len ? len : n);
	if ((ret = (char*)malloc(len + 1)) == NULL)
		return (NULL);
	ret[len] = '\0';
	ft_memcpy(ret, s1, len);
	return (ret);
}
