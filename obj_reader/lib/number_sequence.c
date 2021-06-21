/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number_sequence.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 00:18:48 by vneelix           #+#    #+#             */
/*   Updated: 2021/06/22 00:27:28 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_number_sequence(const char *str, unsigned char sp, unsigned long *bytes)
{
	int	num;
	int	offset;

	num = 0;
	if (bytes)
		*bytes = (unsigned long)str;
	while (*str)
	{
		offset = is_number(str);
		if (!offset)
			return (-1);
		num++;
		str += offset;
		if (*str != sp)
			break ;
		str++;
		while (*str == sp)
		{
			num++;
			str++;
		}
	}
	if (bytes)
		*bytes = (unsigned long)str - *bytes;
	return (num);
}

int	write_number_sequence(const char *str, unsigned char sp,
								unsigned long *bytes, void *dest)
{
	int		num;

	num = 0;
	if (bytes)
		*bytes = (unsigned long)str;
	while (*str)
	{
		*((int *)dest) = (int)ft_atof(str);
		dest += sizeof(int);
		str += is_number(str);
		num++;
		if (*str != sp)
			break ;
		str++;
		while (*str == sp)
		{
			*((int *)dest) = 0;
			dest += sizeof(int);
			num++;
			str++;
		}
	}
	if (bytes)
		*bytes = (unsigned long)str - *bytes;
	return (num);
}
