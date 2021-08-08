/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 11:50:38 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/08 19:59:45 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilib.h"

int	ft_atoi(const char *ptr)
{
	return ((int)ft_atof(ptr));
}

long	ft_atol(const char *ptr)
{
	return ((long)ft_atof(ptr));
}

long long	ft_atoll(const char *ptr)
{
	return ((long long)ft_atof(ptr));
}
