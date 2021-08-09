/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_converter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 20:29:01 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/09 19:55:28 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

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
	return (0);
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
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_float4) * (uint32_t)(*data) * 2
		+ sizeof(t_float2) * (uint32_t)(*data), data + 1, GL_STATIC_DRAW);
	if (glGetError() != 0)
		return (-1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
	if (glGetError() != 0)
		return (-1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		(const GLvoid *)(sizeof(t_float4) * (uint32_t)(*data)));
	if (glGetError() != 0)
		return (-1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
		(const GLvoid *)(sizeof(t_float4) * (uint32_t)(*data) * 2));
	if (glGetError() != 0)
		return (-1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return (0);
}

int	wavefront_to_vao(void **data,
	void **object, GLuint *arr, char **err)
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
			return (error_callback(-1, ((void **)((uint32_t *)
						object + 1))[i], "failed to triangulate", err));
		rewriter(data, ptr);
		arr[1 + i] = *ptr;
		ret = write_to_vao(arr[1 + arr[0] + i], arr[1 + 2 * arr[0] + i], ptr);
		free(ptr);
		if (ret)
			return (error_callback(-1, ((void **)((uint32_t *)
						object + 1))[i], "failed to write data to VAO", err));
		i++;
	}
	return (0);
}

GLuint	*wavefront_to_gl_arrays_converter(
		void **data, void **object, char **err)
{
	GLuint		*arr;

	arr = calloc(
			sizeof(uint32_t) + sizeof(GLuint) * *(uint32_t *)object * 3, 1);
	if (arr == NULL)
		return ((void *)(uint64_t)error_callback(
				0, "VAO", "failed to allocate memory", err));
	*arr = *(uint32_t *)object;
	glGenVertexArrays(*(uint32_t *)object, arr + 1 + *(uint32_t *)object);
	glGenBuffers(*(uint32_t *)object, arr + 1 + *(uint32_t *)object * 2);
	if (wavefront_to_vao(data, object, arr, err))
		return (NULL);
	return (arr);
}
