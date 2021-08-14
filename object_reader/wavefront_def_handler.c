/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_def_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 23:18:43 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/31 04:08:32 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wavefront_object_reader.h"

static int	def_handler2(const char *definition,
				unsigned int *out_def, size_t *out_memory_size)
{
	int	ret;

	if (!strncmp(definition, "f ", 2) || !strncmp(definition, "f\t", 2))
	{
		ret = is_definition_complex(definition, 1);
		if (ret < 3)
			return (-1);
		out_def[f]++;
		if (out_memory_size != NULL)
			*out_memory_size += sizeof(void *)
				+ sizeof(int) + ret * 3 * sizeof(int);
	}
	else if (!strncmp(definition, "vt ", 3) || !strncmp(definition, "vt\t", 3))
	{
		ret = is_definition(definition, 2);
		if (!(ret == 2 || ret == 3))
			return (-1);
		out_def[vt]++;
	}
	return (0);
}

int	def_handler(const char *definition,
	unsigned int *out_def, size_t *out_memory_size)
{
	int	ret;

	if (!strncmp(definition, "v ", 2) || !strncmp(definition, "v\t", 2))
	{
		ret = is_definition(definition, 1);
		if (!(ret == 3 || ret == 4))
			return (-1);
		out_def[v]++;
	}
	else if (!strncmp(definition, "vn ", 3) || !strncmp(definition, "vn\t", 3))
	{
		ret = is_definition(definition, 2);
		if (!(ret == 3 || ret == 4))
			return (-1);
		out_def[vn]++;
	}
	else
		return (def_handler2(definition, out_def, out_memory_size));
	return (0);
}
