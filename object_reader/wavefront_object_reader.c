/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_object_reader.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 22:25:46 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/09 20:15:27 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wavefront_object_reader.h"

static void	cleanup_definition(char *definition)
{
	char	*ptr;

	ptr = definition;
	if (*definition)
		definition += strlen(definition) - 1;
	while (definition >= ptr && (*definition == ' '
			|| *definition == '\t' || *definition == '\r'))
	{
		*definition = 0;
		definition--;
	}
}

static char	**split_wavefront_object(const char *path, char **err)
{
	unsigned int	i;
	char			*file;
	char			**wavefront_object_split;

	file = ft_read_file_fstat(path);
	if (!file)
		return ((char **)(uint64_t)error_callback(
				0, path, "failed to read file", err));
	wavefront_object_split = ft_strsplit(file, '\n');
	free(file);
	if (wavefront_object_split == NULL)
		return ((char **)(uint64_t)error_callback(
				0, path, "failed to split file", err));
	i = 0;
	while (wavefront_object_split[i] != NULL)
		cleanup_definition(wavefront_object_split[i++]);
	return (wavefront_object_split);
}

void	*wavefront_object_reader(const char *path,
			void **out_object, unsigned int **out_def_count, char **out_err)
{
	void			*ret;
	char			**wf_file;
	size_t			def_f_size;
	unsigned int	def_count[4];
	void			*container[2];

	wf_file = split_wavefront_object(path, out_err);
	if (wf_file == NULL)
		return (NULL);
	def_f_size = 0;
	container[0] = &def_f_size;
	*(__uint128_t *)def_count = 0;
	container[1] = (void *)def_count;
	ret = wavefront_object_get(
			(const char **)wf_file, container, out_object, out_err);
	if (out_def_count)
		*(__uint128_t *)out_def_count = *(__uint128_t *)def_count;
	free(wf_file);
	return (ret);
}
