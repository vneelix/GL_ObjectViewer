/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulate_object.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:16:25 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/06 00:30:10 by vneelix          ###   ########.fr       */
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

int	init_vbo_rw(void **vbo_writer, void **idx_reader, int *index_array, float *out_vbo)
{
	__uint32_t	v_count;

	v_count = *index_array * 3;
	vbo_writer[0] = out_vbo;
	vbo_writer[1] = vbo_writer[0] + sizeof(t_float4) * v_count;
	vbo_writer[2] = vbo_writer[1] + sizeof(t_float4) * v_count;
	idx_reader[0] = index_array + 1;
	idx_reader[1] = idx_reader[0] + sizeof(int) * v_count;
	idx_reader[2] = idx_reader[1] + sizeof(int) * v_count;
	return (0);
}

int	write_float4(void *arr, size_t length, void *dest, int idx)
{
	const void	*src;

	src = get_elem_from_array(arr, length, sizeof(t_float4), idx);
	if (src != NULL)
		memcpy(dest, src, sizeof(t_float4));
}

int	write_float2(void *arr, size_t length, void *dest, int idx)
{
	const void	*src;

	src = get_elem_from_array(arr, length, sizeof(t_float2), idx);
	if (src != NULL)
		memcpy(dest, src, sizeof(t_float2));
}

int	write_vbo(void **object_container,
	__uint32_t *def_count, int *index_array, float *out_vbo)
{
	__uint32_t	i;
	void		*vbo_writer[3];
	void		*idx_reader[3];

	i = 0;
	init_vbo_rw(vbo_writer, idx_reader, index_array, out_vbo);
	while (i != *index_array * 3)
	{
		if (def_count[0] != 0)
			write_float4(object_container[0], def_count[0],
				vbo_writer[0], ((int *)(idx_reader[0]))[i] - 1 - def_count[4]);
		if (def_count[1] != 0)
			write_float4(object_container[1], def_count[1],
				vbo_writer[1], ((int *)(idx_reader[2]))[i] - 1 - def_count[5]);
		if (def_count[2] != 0)
			write_float2(object_container[2], def_count[2],
				vbo_writer[2], ((int *)(idx_reader[1]))[i] - 1 - def_count[6]);
		vbo_writer[0] += sizeof(t_float4);
		vbo_writer[1] += sizeof(t_float4);
		vbo_writer[2] += sizeof(t_float2);
		i++;
	}
	return (0);
}

float	*create_vbo(void **object_container, __uint32_t *def_count, int *index_array)
{
	float	*vbo;

	vbo = calloc(sizeof(float) + sizeof(t_float4) * (*index_array * 3) * 2 + sizeof(t_float2) * (*index_array * 3), 1);
	if (vbo == NULL)
		return (NULL);
	*vbo = *index_array * 3;
	write_vbo(object_container, def_count, index_array, vbo + 1);
	return (vbo);
}

float	*wavefront_to_gl_vbo_converter(void **object_container, __uint32_t	*def_count)
{

	__uint32_t	vertex_count = def_count[0];
	t_float4	*vertex = object_container[0];

	__uint32_t	polygon_count = def_count[3];
	void		**polygon = object_container[3];

	int	*index_array;

	index_array = wavefront_to_gl_index_converter(vertex, vertex_count, polygon, polygon_count);

	if (index_array == NULL)
		return (NULL);

	float *vbo = create_vbo(object_container, def_count, index_array);	

	return (vbo);
}
