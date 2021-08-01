/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getfile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 20:28:14 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/29 05:02:10 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*read_file(int fd)
{
	ssize_t	i;
	char	*ret;
	char	*nptr;
	char	*buff;;

	buff = ft_memalloc(BUFF_SIZE + 1);
	if ((ret = (char*)(ft_memalloc(1))) == NULL)
		return (NULL);
	while ((i = read(fd, buff, BUFF_SIZE)) > 0)
	{
		nptr = ret;
		ret = ft_strjoin(ret, buff);
		free(nptr);
		if (ret == NULL)
			return (NULL);
		ft_bzero(buff, BUFF_SIZE + 1);
	}
	free(buff);
	if (i < 0)
		return (NULL);
	return (ret);
}

char		*ft_getfile(char *name, int fd)
{
	if (name != NULL)
		if ((fd = open(name, O_RDONLY)) == -1)
			return (NULL);
	return (read_file(fd));
}
