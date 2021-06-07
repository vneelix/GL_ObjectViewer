/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 21:14:24 by vneelix           #+#    #+#             */
/*   Updated: 2020/08/20 19:43:18 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len[2];
	char	*string;

	if (!s1 || !s2)
		return (ft_strdup(s1 ? s1 : s2));
	len[0] = ft_strlen(s1);
	len[1] = ft_strlen(s2);
	if (!(string = (char*)malloc(sizeof(char) * (len[0] + len[1] + 1))))
		return (NULL);
	string[len[0] + len[1]] = 0;
	ft_memcpy(string, s1, len[0]);
	ft_memcpy(string + len[0], s2, len[1]);
	return (string);
}
