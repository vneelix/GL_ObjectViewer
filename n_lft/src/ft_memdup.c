/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 12:38:33 by vneelix           #+#    #+#             */
/*   Updated: 2020/08/24 12:43:36 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memdup(const void *mem, size_t size)
{
	void	*ret;

	if (!mem || !size)
		return (NULL);
	if (!(ret = malloc(size)))
		return (NULL);
	return (
		ft_memcpy(ret, mem, size));
}
