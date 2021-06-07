/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 20:28:28 by vneelix           #+#    #+#             */
/*   Updated: 2021/05/16 14:59:56 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t n)
{
	void	*memory_area;

	memory_area = malloc(n);
	if (!memory_area)
		return (NULL);
	ft_bzero(memory_area, n);
	return (memory_area);
}
