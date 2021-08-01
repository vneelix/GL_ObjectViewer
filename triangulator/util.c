/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 16:54:24 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/30 20:22:27 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

long	in_range(long range, long index)
{
	if (index >= 0)
		return (index % range);
	else
		return ((range - (-index - 1) % range) - 1);
}

const void	*get_elem_from_array(const void *array, size_t length,
									size_t sizeof_elem, long index)
{
	if (!array || !length || !sizeof_elem)
		return (NULL);
	if (index >= 0)
		return (array + sizeof_elem * (index % length));
	else
		return (array + sizeof_elem * ((length - (-index - 1) % length) - 1));
}

int	get_elem_from_int(const int *array, size_t length, long index)
{
	const int	*ptr;

	ptr = (const int *)get_elem_from_array(array, length, sizeof(int), index);
	if (ptr == NULL)
		return (0);
	return (*ptr);
}

t_float2	get_elem_from_float2(const t_float2 *array,
									size_t length, long index)
{
	const t_float2	*ptr;

	ptr = (const t_float2 *)get_elem_from_array(
			array, length, sizeof(t_float2), index);
	if (ptr == NULL)
		return ((t_float2){0, 0});
	return (*ptr);
}

t_float4	get_elem_from_float4(const t_float4 *array,
									size_t length, long index)
{
	const float	*ptr;

	ptr = (const float *)get_elem_from_array(
			array, length, sizeof(t_float4), index);
	if (ptr == NULL)
		return ((t_float4){0, 0, 0, 0});
	return ((t_float4){ptr[0], ptr[1], ptr[2], ptr[3]});
}
