/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 11:50:38 by vneelix           #+#    #+#             */
/*   Updated: 2020/08/14 13:25:01 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *nptr)
{
	int		num;
	int		sign;

	num = 0;
	sign = 1;
	while (*nptr != '\0' && (*nptr == ' '
		|| *nptr == '\t' || *nptr == '\n'))
		nptr += 1;
	if (*nptr == '-' || *nptr == '+')
		if (*nptr++ == '-')
			sign = -1;
	while (ft_isdigit(*nptr))
	{
		if (sign == 1)
			if ((num * 10 + (*nptr - '0') * sign) <= num)
				return (0);
		if (sign == -1)
			if ((num * 10 + (*nptr - '0') * sign) >= num)
				return (0);
		num = num * 10 + (*nptr - '0') * sign;
		nptr += 1;
	}
	return (num);
}
