/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 21:46:31 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/12 20:57:04 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

double	clamp(double v, double l, double r)
{
	if (v < l)
		return (l);
	else if (v > r)
		return (r);
	else
		return (v);
}

t_float4	cross(t_float4 a, t_float4 b)
{
	return ((t_float4){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
		0
	});
}
