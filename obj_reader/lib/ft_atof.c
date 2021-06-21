/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 15:21:08 by vneelix           #+#    #+#             */
/*   Updated: 2021/06/22 00:14:35 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Convert a string to a double
 * @param str pointer to string
 * @return converted value
*/
double	ft_atof(const char *str)
{
	int		d;
	double	sign;
	double	integer;
	double	fractional;

	d = 0;
	sign = 1;
	integer = 0;
	fractional = 0;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	while (*str >= '0' && *str <= '9')
		integer = integer * 10 + (*str++ - '0') * sign;
	if (*str == '.')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		fractional = fractional * 10 + (*str++ - '0') * sign;
		d--;
	}
	return (integer + fractional * pow(10, d));
}
