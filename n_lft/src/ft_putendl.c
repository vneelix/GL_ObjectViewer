/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:55:56 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/25 04:27:56 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t		ft_putendl(const char *s)
{
	ssize_t	ret;

	if ((ret = ft_putstr(s)) == -1)
		return (-1);
	if (ft_putchar('\n') == -1)
		return (-1);
	return (ret + 1);
}
