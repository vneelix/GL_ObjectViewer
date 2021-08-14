/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbo_rewriter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 20:30:04 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 11:42:10 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

static t_float4	calc_triangle_normal(t_float4 p1, t_float4 p2, t_float4 p3)
{
	t_float4	u;
	t_float4	v;

	u = p2 - p1;
	v = p3 - p2;
	return (normalize(cross(u, v)));
}

static int	rewrite_normal(float *data, uint32_t i)
{
	float		*p;
	t_float4	normal;
	t_float4	triangle[3];

	p = data + 1;
	memcpy(triangle, p + i * 4, sizeof(t_float4) * 3);
	normal = calc_triangle_normal(triangle[0], triangle[1], triangle[2]);
	memcpy(p + (uint32_t)(
			*data) * 4 + i * 4 + 0 * 4, &normal, sizeof(t_float4));
	memcpy(p + (uint32_t)(
			*data) * 4 + i * 4 + 1 * 4, &normal, sizeof(t_float4));
	memcpy(p + (uint32_t)(
			*data) * 4 + i * 4 + 2 * 4, &normal, sizeof(t_float4));
	return (0);
}

static int	rewrite_texture_coord(float *data, uint32_t i)
{
	t_float2	triangle[6];

	triangle[0] = (t_float2){1, 1};
	triangle[1] = (t_float2){0, 0};
	triangle[2] = (t_float2){0, 1};
	triangle[3] = (t_float2){0, 0};
	triangle[4] = (t_float2){1, 1};
	triangle[5] = (t_float2){1, 0};
	memcpy(&data[1 + (uint32_t)(*data) * 8 + i * 2],
		triangle + 3 * (i % 2), sizeof(t_float2) * 3);
	return (0);
}

int	rewriter(void **data, float *vbo)
{
	uint32_t	i;

	if (*(float *)data[vn] != 0
		&& *(float *)data[vt] != 0)
		return (0);
	i = 0;
	while (i != (uint32_t)(*vbo))
	{
		if (*(float *)data[vn] == 0)
			rewrite_normal(vbo, i);
		if (*(float *)data[vt] == 0)
			rewrite_texture_coord(vbo, i);
		i += 3;
	}
	return (0);
}
