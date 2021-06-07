/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 18:17:40 by vneelix           #+#    #+#             */
/*   Updated: 2020/07/08 18:52:18 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_read_directory(char *directory, char **dir, size_t file_counter)
{
	size_t	i;
	char	**file;
	char	*file_path;

	i = 0;
	if (!(file = (char**)malloc(sizeof(char*) * (file_counter + 1))))
		return (NULL);
	ft_bzero(file, sizeof(char**) * (file_counter + 1));
	while (i != file_counter)
	{
		if (!(file_path = ft_strjoin(directory, dir[i])))
			return (ft_nptr_del((void**)file));
		if (!(file[i] = ft_getfile(file_path, 0)))
		{
			free(file_path);
			return (ft_nptr_del((void**)file));
		}
		free(file_path);
		i += 1;
	}
	return (file);
}

int		ft_get_directory(char *directory,
	void **dir, void **dir_file, size_t *count)
{
	char	**ptr_dir;
	char	**ptr_file;
	size_t	file_counter;

	file_counter = 0;
	if (!(ptr_dir = ft_directory(directory, &file_counter)))
		return (-1);
	if (!(ptr_file = ft_read_directory(directory, ptr_dir, file_counter)))
	{
		ft_nptr_del((void**)ptr_dir);
		return (-1);
	}
	*dir = ptr_dir;
	*dir_file = ptr_file;
	*count = file_counter;
	return (0);
}
