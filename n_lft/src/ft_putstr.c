/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:50:33 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/25 04:31:11 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t		ft_putstr(char const *s)
{
	if (s == NULL)
		return (0);
	return (write(1, s, ft_strlen(s)));
}