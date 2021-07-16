/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inside_triangle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 16:45:39 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/13 17:01:44 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

static int	inside_triangle_barycentric(
	t_float4 barycentric_basis, t_float2 p, t_float2 a)
{
	float		c;
	t_float2	coeff_m;

	coeff_m = mul(invert(barycentric_basis), a - p);
	c = 1.f - coeff_m[0] - coeff_m[1];
	if (!(c > 0.f && c < 1.f)
		|| !(coeff_m[0] > 0.f && coeff_m[0] < 1.f)
		|| !(coeff_m[1] > 0.f && coeff_m[1] < 1.f))
		return (0);
	return (1);
}

int	inside_triangle(t_float2 p0, t_float2 p1, t_float2 p2, t_float2 a)
{
	return (inside_triangle_barycentric((t_float4){
			p0.x - p2.x, p1.x - p2.x, p0.y - p2.y, p1.y - p2.y}, p2, a));
}
