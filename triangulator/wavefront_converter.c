/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_converter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 20:55:54 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/07 23:58:13 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

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
	init_writer((void **)writer, index_array, tre_count);
	while (i != polygon_count)
	{
		if (polygon_handling(vertex, vertex_count, polygon[i], writer) == -1)
		{
			free(index_array);
			return (NULL);
		}
		i++;
	}
	*index_array = (int)tre_count * 3;
	return (index_array);
}

int	write_elements_into_vbo(void **data, int *index_array, float *vbo)
{
	int			i;
	const float	*ptr;

	i = 0;
	while (i != *index_array)
	{
		ptr = get_elem_from_array(data[v] + sizeof(float),
				*(float *)(data[v]), sizeof(t_float4), index_array[1 + i] - 1);
		memcpy(&vbo[1 + i * 4], ptr, sizeof(t_float4));
		ptr = get_elem_from_array(data[vn] + sizeof(float),
				*(float *)(data[vn]), sizeof(t_float4),
				index_array[1 + i + *index_array * 2] - 1);
		if (ptr != NULL)
			memcpy(&vbo[1 + (i + *index_array) * 4], ptr, sizeof(t_float4));
		ptr = get_elem_from_array(data[vt] + sizeof(float),
				*(float *)(data[vt]), sizeof(t_float2),
				index_array[1 + i + *index_array] - 1);
		if (ptr != NULL)
			memcpy(&vbo[1 + (i + *index_array * 2) * 4], ptr, sizeof(t_float2));
		i++;
	}
}

float	*wavefront_to_gl_vbo_converter(void **data, uint32_t *object_data)
{
	float	*vbo;
	int		*index_array;

	index_array = wavefront_to_gl_index_converter(
			data[v] + sizeof(float), *(float *)(data[v]),
			data[f] + sizeof(int) + sizeof(void *) * object_data[7],
			object_data[3]);
	if (index_array == NULL)
		return (NULL);
	vbo = calloc(sizeof(float) * *index_array * 3 * sizeof(t_float4), 1);
	if (vbo == NULL)
	{
		free(index_array);
		return (NULL);
	}
	*vbo = *index_array;
	write_elements_into_vbo(data, index_array, vbo);
	free(index_array);
	return (vbo);
}

int	write_to_vao(GLuint vao, GLuint vbo, float *data)
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(t_float4) * (uint32_t)*data * 2
			+ sizeof(t_float2) * (uint32_t)*data, data + 1, GL_STATIC_DRAW);
	if (glGetError() != 0)
		return (-1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
	if (glGetError() != 0)
		return (-1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		(const GLvoid *)(sizeof(t_float4) * (uint32_t)*data));
	if (glGetError() != 0)
		return (-1);
	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
		(const GLvoid *)(sizeof(t_float4) * (uint32_t)*data * 2));*/
	if (glGetError() != 0)
		return (-1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return (0);
}

t_float4 calc_triangle_normal(t_float4 p1, t_float4 p2, t_float4 p3) {
	t_float4 U, V;

	U = p2 - p1;
	V = p3 - p2;
	t_float4 n = normalize(cross(U, V));
	return n;
}

int	rewrite_normal(float *data, uint32_t i)
{
	float		*p;
	t_float4	normal;
	t_float4	triangle[3];

	p = data + 1;
	memcpy(triangle, p + i * 4, sizeof(t_float4) * 3);
	normal = calc_triangle_normal(triangle[0], triangle[1], triangle[2]);
	memcpy(p + (uint32_t)*data * 4 + i * 4 + 0 * 4, &normal, sizeof(t_float4));
	memcpy(p + (uint32_t)*data * 4 + i * 4 + 1 * 4, &normal, sizeof(t_float4));
	memcpy(p + (uint32_t)*data * 4 + i * 4 + 2 * 4, &normal, sizeof(t_float4));
	return (0);
}

int	rewriter(int normal_flag, int textrure_coord_flag, float *data)
{
	uint32_t	i;

	if (!normal_flag && !textrure_coord_flag)
		return (0);
	i = 0;
	while (i != (uint32_t)*data)
	{
		if (normal_flag)
			rewrite_normal(data, i);
		i += 3;
	}
	return (0);
}

int	wavefront_to_vao(void **data, void **object, GLuint *arr, char *err)
{
	uint32_t	i;
	int			ret;
	float		*ptr;

	i = 0;
	while (i != *(uint32_t *)object)
	{
		ptr = wavefront_to_gl_vbo_converter(data,
			((void **)((uint32_t *)object + 1))[i] + MAX_NAME_LEN);
		if (ptr == NULL)
			return (-1);
		rewriter(1, 0, ptr);
		arr[1 + i] = *ptr;
		ret = write_to_vao(arr[1 + arr[0] + i], arr[1 + 2 * arr[0] + i], ptr);
		free(ptr);
		if (ret)
			return (-1);
		i++;
	}
	return (0);
}

GLuint	*wavefront_to_gl_arrays_converter(void **data, uint32_t *def_count, void **object, char *err)
{
	GLuint		*arr;

	arr = calloc(sizeof(uint32_t) + sizeof(GLuint) * *(uint32_t *)object * 3, 1);
	if (arr == NULL)
		return (NULL);
	*arr = *(uint32_t *)object;
	glGenVertexArrays(*(uint32_t *)object, arr + 1 + *(uint32_t *)object);
	glGenBuffers(*(uint32_t *)object, arr + 1 + *(uint32_t *)object * 2);
	if (wavefront_to_vao(data, object, arr, err)) {
		return (NULL);
	}
	return (arr);
}
