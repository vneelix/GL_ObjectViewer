/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 20:27:17 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/23 20:27:27 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	int		i;
	int		sign;
	char	storage[12];

	i = 10;
	ft_bzero(storage, 12);
	sign = (n >= 0 ? 1 : -1);
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
