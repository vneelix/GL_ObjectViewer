/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/06 21:12:35 by vneelix           #+#    #+#             */
/*   Updated: 2020/08/27 15:19:09 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*ret;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	if ((ret = (char*)malloc(len + 1)) == NULL)
		return (NULL);
	ft_memcpy(ret, s1, len + 1);
	return (ret);
}
