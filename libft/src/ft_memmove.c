/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 20:43:44 by vneelix           #+#    #+#             */
/*   Updated: 2019/09/08 15:44:10 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*ret;

	if ((ret = malloc(n)) == NULL)
		return (NULL);
	ft_memcpy(ret, src, n);
	ft_memcpy(dest, ret, n);
	free(ret);
	return (dest);
}
