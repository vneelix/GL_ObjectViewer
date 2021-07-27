/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_object_reader.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 22:25:46 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/26 19:16:17 by vneelix          ###   ########.fr       */
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

static char	**split_wavefront_object(const char *path)
{
	unsigned int	i;
	char			*file;
	char			**wavefront_object_split;

	file = ft_read_file_fstat(path);
	if (!file)
		return (NULL);
	wavefront_object_split = ft_strsplit(file, '\n');
	free(file);
	if (wavefront_object_split == NULL)
		return (NULL);
	i = 0;
	while (wavefront_object_split[i] != NULL)
		cleanup_definition(wavefront_object_split[i++]);
	return (wavefront_object_split);
}

int	error_callback(int err_code, const char *err_message, char **out_err)
{
	if (out_err != NULL
		&& err_message != NULL)
	{
		*out_err = strdup(err_message);
	}
	return (err_code);
}

void	*wavefront_object_reader(const char *path,
			void **out_object, unsigned int **out_def_count, char **out_err)
{
	void			*ret;
	char			**wf_file;
	size_t			def_f_size;
	unsigned int	def_count[4];
	void			*container[2];

	wf_file = split_wavefront_object(path);
	if (wf_file == NULL)
		return ((void *)(uint64_t)error_callback(0,
			"Split of the incoming file failed", out_err));
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
