/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/20 16:02:20 by vneelix           #+#    #+#             */
/*   Updated: 2020/08/20 20:31:58 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	getline(char *buff, char **line)
{
	char	*ptr;
	char	*temp;
	size_t	length;

	if (!(ptr = ft_memchr(buff, '\n', ft_strlen(buff))))
	{
		if (!(temp = ft_strjoin(*line, buff)))
			return (-1);
		ft_memdel((void**)line);
		*line = temp;
		ft_bzero(buff, BUFF_SIZE);
	}
	else
	{
		*ptr = '\0';
		length = ft_strlen(ptr + 1);
		if (!(temp = ft_strjoin(*line, buff)))
			return (-1);
		ft_memdel((void**)line);
		*line = temp;
		ft_memcpy(buff, ptr + 1, length);
		ft_bzero(buff + length, ft_strlen(buff + length));
		return (1);
	}
	return (0);
}

static int	readline(const int fd, char *buff, char **line)
{
	int		ret;
	ssize_t num;

	ret = 0;
	if (*buff && (ret = getline(buff, line)))
		return (ret);
	while ((num = read(fd, buff, BUFF_SIZE)) > 0)
	{
		if ((ret = getline(buff, line)))
			return (ret);
	}
	return (num);
}

int			ft_get_next_line(const int fd, char **line)
{
	int			ret;
	static char	*buff = NULL;

	if (fd < 0 || !line)
		return (-1);
	if (!buff && !(buff = ft_memalloc(BUFF_SIZE + 1)))
		return (-1);
	*line = NULL;
	ret = readline(fd, buff, line);
	if (ret < 0 || ret == 0)
		ft_memdel((void**)&buff);
	if (ret < 0)
		ft_memdel((void**)line);
	return (ret);
}
