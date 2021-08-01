/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 20:29:39 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/23 20:29:40 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_ultoa(unsigned long n)
{
	int		i;
	char	storage[21];

	i = 19;
	ft_bzero(storage, 21);
	while (n != 0 || (i == 19 && n == 0))
	{
		storage[i--] = n % 10 + '0';
		n /= 10;
	}
	i += 1;
	return (ft_strdup(storage + i));
}
