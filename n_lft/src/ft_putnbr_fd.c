/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 20:59:10 by vneelix           #+#    #+#             */
/*   Updated: 2020/04/25 04:32:23 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t		ft_putnbr_fd(int n, int fd)
{
	char	*s;
	ssize_t	ret;

	if ((s = ft_itoa(n)) == NULL)
		return (-1);
	ret = ft_putstr_fd(s, fd);
	free(s);
	return (ret);
}
