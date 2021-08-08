/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:16:25 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/08 20:40:35 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

static int	convert_indexes(int *polygon, int **writer)
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
		polygon3d[i] = get_elem_from_float4(vertex,
				vertex_count, (polygon + 1)[i * 3] - 1);
		i++;
	}
	if (!triangulate_polygon3d(polygon3d, (size_t)polygon[0], writer[0]))
		return (-1);
	convert_indexes(polygon, writer);
	return (0);
}

static int	init_writer(void **writer, int *index_array, size_t tre_count)
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
