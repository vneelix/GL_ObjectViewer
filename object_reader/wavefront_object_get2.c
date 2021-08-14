/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_object_get2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 18:52:15 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/09 21:32:04 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wavefront_object_reader.h"

static int	write_object_name(const char *definition, void *out_object)
{
	size_t	len;

	if (*definition)
		definition++;
	while (*definition == ' '
		|| *definition == '\t')
		definition++;
	len = strlen(definition);
	bzero(out_object, MAX_NAME_LEN);
	if (len < MAX_NAME_LEN)
		memcpy(out_object, definition, len);
	else
		memcpy(out_object, definition, MAX_NAME_LEN - 1);
	return (0);
}

int	change_object(const char *definition,
		void **out_object, int rewrite_flag, char **out_err)
{
	unsigned int	*def;
	unsigned int	offset[4];

	def = (unsigned int *)(out_object[0] + MAX_NAME_LEN);
	if ((def[v] == 0 || def[f] == 0)
		&& (def[vn] != 0 || def[vt] != 0))
		return (error_callback(-1, definition,
				"incomplete object (v == 0 or f == 0)", out_err));
	if (def[v] == 0 && def[f] == 0)
	{
		if (rewrite_flag == 0)
			write_object_name(definition, out_object[0]);
		return (0);
	}
	if (rewrite_flag == MAX_OBJ_COUNT - 1)
		return (error_callback(
				-1, definition, "limit of objects has been reached", out_err));
	write_object_name(definition, out_object[1]);
	offset[0] = def[0] + def[4 + 0];
	offset[1] = def[1] + def[4 + 1];
	offset[2] = def[2] + def[4 + 2];
	offset[3] = def[3] + def[4 + 3];
	memcpy(out_object[1] + MAX_NAME_LEN
		+ sizeof(unsigned int) * 4, offset, sizeof(unsigned int) * 4);
	return (1);
}

void	*create_object_buffer(char **out_err)
{
	unsigned int	index;
	void			**object;
	void			**object_buffer;

	object_buffer = calloc(sizeof(unsigned int)
			+ (sizeof(void *) + MAX_NAME_LEN + 8
				* sizeof(unsigned int)) * MAX_OBJ_COUNT + sizeof(void *), 1);
	if (object_buffer == NULL)
		return ((void *)(uint64_t)error_callback(
				0, "array of \'o\' definitions",
				"failed to allocate memory", out_err));
	index = 0;
	object = (void *)object_buffer + sizeof(unsigned int);
	while (index != MAX_OBJ_COUNT)
	{
		object[index] = (void *)(object + MAX_OBJ_COUNT + 1)
			+ (MAX_NAME_LEN + 8 * sizeof(unsigned int)) * index;
		index++;
	}
	memcpy(object[0], "default", strlen("default"));
	return (object_buffer);
}
