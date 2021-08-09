/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_callback.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 19:56:03 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/09 19:56:51 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilib.h"

int	error_callback(int err_code,
	const char *target, const char *reason, char **err)
{
	char	*buff;
	size_t	target_len;
	size_t	reason_len;

	if (target == NULL || reason == NULL || err == NULL)
		return (err_code);
	target_len = strlen(target);
	reason_len = strlen(reason);
	buff = (char *)calloc(target_len + reason_len + 2, 1);
	if (buff == NULL)
	{
		if (err)
			*err = NULL;
		return (err_code);
	}
	buff[target_len] = ':';
	memcpy(buff, target, target_len);
	memcpy(buff + target_len + 1, reason, reason_len);
	*err = buff;
	return (err_code);
}
