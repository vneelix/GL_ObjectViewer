/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 13:04:22 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 13:04:58 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilib.h"

char	*ft_itoa(int n)
{
	int		i;
	int		sign;
	char	storage[12];

	i = 10;
	ft_bzero(storage, 12);
	if (n >= 0)
		sign = 1;
	else
		sign = 0;
	while (n != 0 || (i == 10 && n == 0))
	{
		storage[i--] = n % 10 * sign + '0';
		n /= 10;
	}
	if (sign != 1)
		storage[i--] = '-';
	i += 1;
	return (ft_strdup(storage + i));
}
