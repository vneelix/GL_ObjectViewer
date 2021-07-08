/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definition.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 23:45:06 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/06 22:07:19 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_reader.h"

/* [char sequence] float_0 ... float_n-1 float_n */

int	is_definition(const char *str, unsigned int offset)
{
	int	coord;

	coord = 0;
	str += offset;
	if (*str != ' ' && *str != '\t')
		return (0);
	while (*str)
	{
		while (*str == ' ' || *str == '\t')
			str++;
		offset = is_number(str);
		if (!offset)
			return (-1);
		coord++;
		str += offset;
	}
	return (coord);
}

/* [char sequence] float_0 ... float_n-1 float_n */
int	write_definition(const char *str, unsigned int offset, float *dest)
{
	str += offset;
	while (*str)
	{
		while (*str == ' ' || *str == '\t')
			str++;
		*dest++ = ft_atof(str);
		while (*str && *str != ' ' && *str != '\t')
			str++;
	}
	return (0);
}

/* [char sequence] float_0/float_0/float_0
... float_n-1/float_n-1/float_n-1 float_n/float_n/float_n */
int	is_definition_complex(const char *str, unsigned int offset)
{
	int				num;
	int				coord;
	unsigned long	bytes;

	coord = 0;
	str += offset;
	if (*str != ' ' && *str != '\t')
		return (-1);
	while (*str)
	{
		while (*str == ' ' || *str == '\t')
			str++;
		num = is_number_sequence(str, '/', &bytes);
		if (!(num ==1 || num == 3))
			return (-1);
		str += bytes;
		coord++;
	}
	return (coord);
}

/* [char sequence] float_0/float_0/float_0
... float_n-1/float_n-1/float_n-1 float_n/float_n/float_n */
void	*write_definition_complex(const char *str,
									unsigned int offset, void *dest)
{
	int				count;
	unsigned long	bytes;

	count = is_definition_complex(str, 1);
	*((int *)dest) = count;
	dest += sizeof(int);
	str += offset;
	while (*str)
	{
		while (*str == ' ' || *str == '\t')
			str++;
		write_number_sequence(str, '/', &bytes, dest);
		dest += sizeof(int) * 3;
		str += bytes;
	}
	return (dest);
}
