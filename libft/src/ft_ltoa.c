/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 20:27:39 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/23 20:27:41 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_ltoa(long n)
{
	int		i;
	int		sign;
	char	storage[21];

	i = 19;
	ft_bzero(storage, 21);
	sign = (n >= 0 ? 1 : -1);
	while (n != 0 || (i == 19 && n == 0))
	{
		storage[i--] = n % 10 * sign + '0';
		n /= 10;
	}
	if (sign != 1)
		storage[i--] = '-';
	i += 1;
	return (ft_strdup(storage + i));
}
