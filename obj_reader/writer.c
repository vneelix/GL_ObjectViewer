/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 23:17:15 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/15 23:17:35 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_reader.h"

static int	simple_writer(const char *str, void *dest, unsigned int def)
{
	static void	*writer[3] = {NULL, NULL, NULL};

	if (!writer[def])
		writer[def] = dest;
	if (def == v)
	{
		write_definition(str, 1, writer[def]);
		((float *)writer[def])[3] = 1.;
	}
	else
		write_definition(str, 2, writer[def]);
	writer[def] += sizeof(float) * 4;
	return (0);
}

static int	complex_writer(const char *str, void *dest, unsigned long count)
{
	static void	*writer_ptr = NULL;
	static void	*writer_data = NULL;

	if (!writer_ptr)
		writer_ptr = dest;
	if (!writer_data)
		writer_data = dest + sizeof(void *) * (count + 1);
	*((void **)writer_ptr) = writer_data;
	writer_ptr += sizeof(void *);
	writer_data = write_definition_complex(str, 1, writer_data);
	return (0);
}

int	def_writer(const char **obj, void *mem, unsigned long *def)
{
	while (*obj)
	{
		if (!strncmp(*obj, "v ", 2) || !strncmp(*obj, "v\t", 2))
			simple_writer(*obj, ((void **)mem)[v] + sizeof(float), v);
		else if (!strncmp(*obj, "vn ", 3) || !strncmp(*obj, "vn\t", 3))
			simple_writer(*obj, ((void **)mem)[vn] + sizeof(float), vn);
		else if (!strncmp(*obj, "vt ", 3) || !strncmp(*obj, "vt\t", 3))
			simple_writer(*obj, ((void **)mem)[vt] + sizeof(float), vt);
		else if (!strncmp(*obj, "f ", 2) || !strncmp(*obj, "f\t", 2))
			complex_writer(*obj, ((void **)mem)[f] + sizeof(int), def[f]);
		obj++;
	}
	return (0);
}
