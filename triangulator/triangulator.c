/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 22:11:36 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/26 22:39:21 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

int	check_collision(void **container, t_float2 *triangle, int *indexes)
{
	size_t		i;
	t_float2	a;

	i = 0;
	while (i != (size_t)(container[1]))
	{
		a = ((t_float2 *)container[0])[i];
		if (*((__uint64_t *)&a) == *((__uint64_t *)triangle + 0))
			indexes[0] = i;
		else if (*((__uint64_t *)&a) == *((__uint64_t *)triangle + 1))
			indexes[1] = i;
		else if (*((__uint64_t *)&a) == *((__uint64_t *)triangle + 2))
			indexes[2] = i;
		else if (inside_triangle(triangle[0], triangle[1], triangle[2], a))
			return (1);
		i++;
	}
	return (0);
}

int	is_valid_triangle(void **container, int index, int *indexes)
{
	t_float2	p[3];
	t_float2	vec0;
	t_float2	vec1;

	p[0] = get_elem_from_float2(container[2], (size_t)container[3], index - 1);
	p[1] = get_elem_from_float2(container[2], (size_t)container[3], index);
	p[2] = get_elem_from_float2(container[2], (size_t)container[3], index + 1);
	vec0 = p[2] - p[0];
	vec1 = p[1] - p[0];
	if (vec0.x * vec1.y - vec0.y * vec1.x <= 0)
		return (0);
	return (!check_collision(container, p, indexes));
}

int	get_triangle(void **container, int *out_triangle)
{
	size_t	i;
	size_t	vertex_buffer_len;

	i = 0;
	vertex_buffer_len = (size_t)(container[3]);
	while (i != vertex_buffer_len)
	{
		if (is_valid_triangle(container, i, out_triangle))
		{
			int64_stack_shift_up(
				(__int64_t *)(container[2]) + i, vertex_buffer_len - i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	triangulate_polygon(t_float2 *polygon,
						size_t vertex_count, int *out_triangle)
{
	void		*container[4];
	size_t		*vertex_buffer_len;
	t_float2	vertex_buffer[VERTEX_BUFFER_SIZE];

	if (vertex_count < 3 || vertex_count > VERTEX_BUFFER_SIZE)
		return (-1);
	container[0] = (void *)polygon;
	container[1] = (void *)vertex_count;
	container[2] = (void *)vertex_buffer;
	container[3] = (void *)vertex_count;
	memcpy(vertex_buffer, polygon, sizeof(t_float2) * vertex_count);
	vertex_buffer_len = (size_t *)(&container[3]);
	while (*vertex_buffer_len > 3)
	{
		if (!get_triangle(container, out_triangle))
			return (0);
		out_triangle += 3;
		(*vertex_buffer_len)--;
	}
	if (!get_triangle(container, out_triangle))
		return (0);
	return (1);
}
