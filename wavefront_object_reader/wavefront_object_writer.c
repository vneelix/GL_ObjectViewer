/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_object_writer.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 23:17:15 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/09 20:45:38 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wavefront_object_reader.h"

static int	simple_writer(const char *str, void *dest, unsigned int def)
{
	static void	*writer[3] = {NULL, NULL, NULL};

	if (str == NULL && dest == NULL && def == 0)
	{
		bzero(writer, sizeof(void *) * 3);
		return (0);
	}
	if (writer[def] == NULL)
		writer[def] = dest;
	if (def == v)
	{
		write_definition(str, 1, writer[def]);
		((float *)writer[def])[3] = 1.;
	}
	else
		write_definition(str, 2, writer[def]);
	if (def != vt)
		writer[def] += sizeof(float) * 4;
	else
		writer[def] += sizeof(float) * 3;
	return (0);
}

static int	complex_writer(const char *str, void *dest, unsigned long count)
{
	static void	**writer_ptr = NULL;
	static void	*writer_data = NULL;

	if (str == NULL && dest == NULL && count == 0)
	{
		writer_ptr = NULL;
		writer_data = NULL;
		return (0);
	}
	if (writer_ptr == NULL)
		writer_ptr = dest;
	if (writer_data == NULL)
		writer_data = dest + sizeof(void *) * (count + 1);
	*writer_ptr = writer_data;
	writer_ptr++;
	writer_data = write_definition_complex(str, 1, writer_data);
	return (0);
}

int	def_writer(const char **obj, void *mem, unsigned int *def)
{
	uint32_t	def_count[4];

	*(__uint128_t *)def_count = *(__uint128_t *)def;
	while (*obj)
	{
		if ((!strncmp(*obj, "v ", 2)
				|| !strncmp(*obj, "v\t", 2)) && def_count[v]--)
			simple_writer(*obj, ((void **)mem)[v] + sizeof(float), v);
		else if ((!strncmp(*obj, "vn ", 3)
				|| !strncmp(*obj, "vn\t", 3)) && def_count[vn]--)
			simple_writer(*obj, ((void **)mem)[vn] + sizeof(float), vn);
		else if ((!strncmp(*obj, "vt ", 3)
				|| !strncmp(*obj, "vt\t", 3)) && def_count[vt]--)
			simple_writer(*obj, ((void **)mem)[vt] + sizeof(float), vt);
		else if ((!strncmp(*obj, "f ", 2)
				|| !strncmp(*obj, "f\t", 2)) && def_count[f]--)
			complex_writer(*obj, ((void **)mem)[f] + sizeof(int), def[f]);
		obj++;
	}
	return (0);
}
