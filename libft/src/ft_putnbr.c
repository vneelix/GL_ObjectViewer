/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 20:01:02 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/25 04:32:03 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t		ft_putnbr(int n)
{
	char	*s;
	ssize_t	ret;

	if ((s = ft_itoa(n)) == NULL)
		return (-1);
	ret = ft_putstr(s);
	free(s);
	return (ret);
}
