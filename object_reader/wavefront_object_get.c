/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_object_get.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 18:44:59 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/11 01:46:48 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wavefront_object_reader.h"

static int	object_definition_handler(const char *definition,
			void **out_object, unsigned int i, char **out_err)
{
	return (change_object(definition, out_object + i, i, out_err));
}

static int	wavefront_object_analyzer(const char **wf_file,
			void **container, void **out_object, char **out_err)
{
	unsigned int	i;
	int				ret;

	i = 0;
	while (*wf_file != NULL)
	{
		if (def_handler(*wf_file,
				out_object[i] + MAX_NAME_LEN, container[0]) == -1)
			return (error_callback(
					-1, *wf_file, "invalid definition", out_err));
		if (!strncmp(*wf_file, "o ", 2) || !strncmp(*wf_file, "o\t", 2))
		{
			ret = object_definition_handler(*wf_file, out_object, i, out_err);
			if (ret == -1)
				return (-1);
			(*((__uint128_t *)container[1])) += *(
					(__uint128_t *)(out_object[i] + MAX_NAME_LEN));
			if (ret == 1)
				i++;
		}
		wf_file++;
	}
	(*((__uint128_t *)container[1])) += *(
			(__uint128_t *)(out_object[i] + MAX_NAME_LEN));
	return (i + 1);
}

static int	wavefront_object_handler(const char **wf_file,
			void **container, void **out_object, char **out_err)
{
	int				ret;
	void			**object_buffer;

	object_buffer = create_object_buffer(out_err);
	if (object_buffer == NULL)
		return (-1);
	ret = wavefront_object_analyzer(wf_file, container,
			(void *)object_buffer + sizeof(unsigned int), out_err);
	if (ret == -1 || ret == 0)
	{
		free(object_buffer);
		return (-1);
	}
	*(unsigned int *)object_buffer = ret;
	*out_object = (void *)object_buffer;
	return (0);
}

static void	*load_wavefront_object(const char **wf_file,
					unsigned int *def_count, size_t size)
{
	void	**mem;

	mem = calloc(
			sizeof(void *) * 5
			+ (sizeof(float) + sizeof(float) * 4 * def_count[v])
			+ (sizeof(float) + sizeof(float) * 4 * def_count[vn])
			+ (sizeof(float) + sizeof(float) * 3 * def_count[vt])
			+ (sizeof(int) + size + sizeof(void *)), 1);
	if (mem == NULL)
		return (NULL);
	mem[v] = mem + 5;
	mem[vn] = mem[v] + sizeof(float) + def_count[v] * sizeof(float) * 4;
	mem[vt] = mem[vn] + sizeof(float) + def_count[vn] * sizeof(float) * 4;
	mem[f] = mem[vt] + sizeof(float) + def_count[vt] * sizeof(float) * 3;
	((float *)mem[v])[0] = def_count[v];
	((float *)mem[vn])[0] = def_count[vn];
	((float *)mem[vt])[0] = def_count[vt];
	((int *)mem[f])[0] = (int)def_count[f];
	def_writer(wf_file, mem, def_count);
	return (mem);
}

void	*wavefront_object_get(const char **wf_file,
			void **container, void **out_object, char **out_err)
{
	void	**object;
	void	**wf_object;

	if (wavefront_object_handler(wf_file,
			container, (void **)&object, out_err) == -1)
		return (NULL);
	wf_object = load_wavefront_object(wf_file,
			container[1], *(size_t *)container[0]);
	if (wf_object == NULL)
	{
		free(object);
		return ((void *)(uint64_t)error_callback(
				0, "wavefront object", "failed to load", out_err));
	}
	if (out_object)
		*out_object = object;
	else
		free(object);
	return (wf_object);
}
