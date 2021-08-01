/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulate_object.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:16:25 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/31 05:40:10 by vneelix          ###   ########.fr       */
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
		size_t polygon3d_vertex_count, int *out_triangle) {
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

size_t	model_triangles_count(int **polygon, size_t polygon_count)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (i != polygon_count)
	{
		if (polygon[i][0] < 3)
			return (0);
		count += polygon[i][0] - 2;
		i++;
	}
	return (count);
}

int	convert_indexes(int *polygon, int **writer)
{
	int	i;
	int	size;
	int	index;

	i = 0;
	size = (polygon[0] - 2) * 3;
	while (i != size)
	{
		index = writer[0][i] * 3;
		writer[0][i] = (polygon + 1)[index];
		writer[1][i] = (polygon + 1)[index + 1];
		writer[2][i] = (polygon + 1)[index + 2];
		i++;
	}
	writer[0] += size;
	writer[1] += size;
	writer[2] += size;
	return (0);
}

int	polygon_handling(t_float4 *vertex,
		size_t vertex_count, int *polygon, int **writer)
{
	size_t		i;
	t_float4	polygon3d[VERTEX_BUFFER_SIZE];

	i = 0;
	if (polygon[0] < 3)
		return (-1);
	while (i != (size_t)polygon[0])
	{
		polygon3d[i] = get_elem_from_float4(
				vertex, vertex_count, (polygon + 1)[i * 3] - 1);
		i++;
	}
	if (!triangulate_polygon3d(polygon3d, (size_t)polygon[0], writer[0]))
		return (-1);
	convert_indexes(polygon, writer);
	return (0);
}

int	init_writer(void **writer, int *index_array, size_t tre_count)
{
	writer[0] = (void *)(index_array + 1);
	writer[1] = (void *)(index_array + 1 + 3 * tre_count);
	writer[2] = (void *)(index_array + 1 + 6 * tre_count);
	return (0);
}

int	*wavefront_to_gl_index_converter(t_float4 *vertex,
		size_t vertex_count, int **polygon, size_t polygon_count)
{
	size_t		i;
	size_t		tre_count;
	int			*writer[3];
	int			*index_array;

	i = 0;
	tre_count = model_triangles_count(polygon, polygon_count);
	if (tre_count == 0)
		return (NULL);
	index_array = (int *)calloc(sizeof(int) + sizeof(int) * tre_count * 9, 1);
	if (!index_array)
		return (NULL);
	init_writer(writer, index_array, tre_count);
	while (i != polygon_count)
	{
		if (i == 1755)
			i = 1755;
		if (polygon_handling(vertex, vertex_count, polygon[i], writer) == -1)
		{
			free(index_array);
			return (NULL);
		}
		i++;
	}
	*index_array = (int)tre_count;
	return (index_array);
}

int	debug_index_array(t_float4 *vertex, t_float4 *normal, float *texture_coord, int *index_array, float *out_vbo);

float	*wavefront_to_gl_vbo_converter(t_float4 *vertex,
			t_float4 *normal, float *texture_coord, void *polygon)
{
	int	*index_array;

	index_array = wavefront_to_gl_index_converter(
		(float *)vertex + 1, ((float *)vertex)[0], (int *)polygon + 1, *(int *)polygon);

	if (index_array == NULL)
		return (NULL);

	size_t vertex_count = *index_array * 3;
	size_t size = sizeof(t_float4) * vertex_count * 2 + sizeof(t_float2) * vertex_count;


	float *vbo = calloc(sizeof(float) + size, 1);
	*vbo = vertex_count;

	debug_index_array(vertex, normal, texture_coord, index_array, vbo + 1);
	

	return (vbo);
}

int	debug_index_array(t_float4 *vertex, t_float4 *normal, float *texture_coord, int *index_array, float *out_vbo) {

	int n_count = *index_array * 3;
	index_array++;

	int *vertex_index = index_array;
	int *texture_index = index_array + n_count;
	int *normal_index = index_array + n_count * 2;

	float *vbo_vertex = out_vbo;
	t_float2 *vbo_texture_coord = (void *)out_vbo + sizeof(t_float4) * n_count;
	float *vbo_normal = (void *)out_vbo + sizeof(t_float4) * n_count + sizeof(t_float2) * n_count;

	for (int i = 0; i != n_count; i++) {
		t_float4 v, n;
		t_float2 t;

		v = get_elem_from_float4((float*)vertex + 1, *(float*)vertex, vertex_index[i] - 1);
		if (normal_index[i] != 0)
			n = get_elem_from_float4((float*)normal + 1, *(float*)normal, normal_index[i] - 1);
		else
			n = (t_float4){0, 0, 0, 0};
		if (texture_index[i] != 0) {
			const t_float2 *ptr_t =
				(const t_float2 *)get_elem_from_array(texture_coord + 1, *texture_coord, sizeof(float) * 3, texture_index[i] - 1);
			if (ptr_t != NULL)
				t = *ptr_t;
			else
				t = (t_float2){0, 0};
		}
		else
			t = (t_float2){0, 0};

		vbo_vertex[0] = v.x;
		vbo_vertex[1] = v.y;
		vbo_vertex[2] = v.z;
		vbo_vertex[3] = v.w;

		vbo_normal[0] = n.x;
		vbo_normal[1] = n.y;
		vbo_normal[2] = n.z;
		vbo_normal[3] = n.w;

		*vbo_texture_coord = t;
		vbo_vertex += 4;
		vbo_normal += 4;
		vbo_texture_coord++;

		/* printf("v[%3d] % f % f % f | ", vertex_index[i], v.x, v.y, v.z);
		printf("vn[%3d] % f % f % f | ", normal_index[i], n.x, n.y, n.z);
		printf("vt[%3d] % f % f\n", texture_index[i], t.x, t.y); */
	}
	return (0);
}

/* int	*triangulate_model(t_float4 *vertex, size_t vertex_count,
							int **polygon, size_t polygon_count)
{
	size_t		i;
	int			*p;
	int			*triangle;
	size_t		triangle_count;

	i = 0;
	triangle_count = model_triangles_count(polygon, polygon_count);
	triangle = (int *)malloc(sizeof(int) + sizeof(int) * 3 * triangle_count);
	if (!triangle)
		return (NULL);
	p = triangle + 1;
	while (i != polygon_count)
	{
		if (extract_and_triangulate_polygon(
				vertex, vertex_count, polygon[i], p, triangle_count) == -1)
		{
			free(triangle);
			return (NULL);
		}
		p += (polygon[i][0] - 2) * 3;
		i++;
	}
	*triangle = (int)triangle_count;
	return (triangle);
} */
