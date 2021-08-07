/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulator_3d.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 20:53:26 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/07 21:33:52 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

t_matrix4x4	get_ortho_frame(t_float4 a, t_float4 b, t_float4 c)
{
	t_float4	i;
	t_float4	j;
	t_float4	k;

	i = normalize(b - a);
	k = normalize(cross(i, c - a));
	j = normalize(cross(i, k));
	return ((t_matrix4x4){
		i.x, j.x, k.x, 0,
		i.y, j.y, k.y, 0,
		i.z, j.z, k.z, 0,
		0, 0, 0, 1
	});
}

int	composition_to_translate(t_float4 *coord_system,
		t_matrix4x4 *out_coord_m, t_matrix4x4 *out_translate_m) {
	t_float4	center;
	t_matrix4x4	coord_m;
	t_matrix4x4	translate_m;

	coord_m = invert(get_ortho_frame(
				coord_system[0], coord_system[1], coord_system[2]));
	center = mul(coord_m, coord_system[0]);
	translate_m = (t_matrix4x4){
		1, 0, 0, -center.x,
		0, 1, 0, -center.y,
		0, 0, 1, -center.z,
		0, 0, 0, 1
	};
	if (out_coord_m)
		*out_coord_m = coord_m;
	if (out_translate_m)
		*out_translate_m = translate_m;
	return (0);
}

int	triangulate_polygon3d(t_float4 *polygon3d,
		size_t polygon3d_vertex_count, int *out_triangle)
{
	size_t		i;
	t_matrix4x4	coord_m;
	t_matrix4x4	translate_m;
	t_float2	polygon2d[VERTEX_BUFFER_SIZE];

	i = 0;
	composition_to_translate(polygon3d, &coord_m, &translate_m);
	while (i != polygon3d_vertex_count)
	{
		polygon2d[i] = mul(translate_m, mul(coord_m, polygon3d[i])).xy;
		i++;
	}
	return (triangulate_polygon(
			polygon2d, polygon3d_vertex_count, out_triangle));
}
