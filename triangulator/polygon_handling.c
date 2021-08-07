/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:16:25 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/07 21:01:41 by vneelix          ###   ########.fr       */
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
