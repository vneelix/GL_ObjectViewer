/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 23:18:43 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/15 23:20:12 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_reader.h"

static int	def_handler2(const char *definition,
				unsigned long *out_def, unsigned long *out_memory_size)
{
	int	ret;

	if (!strncmp(definition, "f ", 2) || !strncmp(definition, "f\t", 2))
	{
		out_def[f]++;
		ret = is_definition_complex(definition, 1);
		if (ret < 0)
			return (-1);
		*out_memory_size += sizeof(void *)
			+ sizeof(int) + ret * 3 * sizeof(int);
	}
	else if (!strncmp(definition, "vt ", 3) || !strncmp(definition, "vt\t", 3))
	{
		out_def[vt]++;
		ret = is_definition(definition, 2);
		if (!(ret == 2 || ret == 3))
			return (-1);
	}
	return (0);
}

int	def_handler(const char *definition,
		unsigned long *out_def, unsigned long *out_memory_size)
{
	int	ret;

	if (!strncmp(definition, "v ", 2) || !strncmp(definition, "v\t", 2))
	{
		out_def[v]++;
		ret = is_definition(definition, 1);
		if (!(ret == 3 || ret == 4))
			return (-1);
	}
	else if (!strncmp(definition, "vn ", 3) || !strncmp(definition, "vn\t", 3))
	{
		out_def[vn]++;
		ret = is_definition(definition, 2);
		if (!(ret == 3 || ret == 4))
			return (-1);
	}
	else
		return (def_handler2(definition, out_def, out_memory_size));
	return (0);
}
