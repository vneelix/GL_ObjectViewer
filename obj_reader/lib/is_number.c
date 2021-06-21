/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 17:28:56 by vneelix           #+#    #+#             */
/*   Updated: 2021/06/20 00:07:59 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_number(const char *str)
{
	const char	*p;
	int			point;

	p = str;
	point = 0;
	if (*str == '.' || *str == '+' || *str == '-')
		if (*str++ == '.')
			point = 1;
	if (!(*str >= '0' && *str <= '9'))
		return (0);
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
		{
			if (*str != '.' || point)
				break ;
			else
				point = 1;
		}
		str++;
	}
	return (str - p);
}
