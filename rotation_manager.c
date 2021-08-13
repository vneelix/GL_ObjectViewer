/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 00:32:59 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 04:47:32 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

t_matrix4x4	rotation_manager(int8_t x, int8_t y)
{
	static float	angle_x = 0;
	static float	angle_y = 0;
	t_matrix4x4		rotation_axisx;
	t_matrix4x4		rotation_axisy;

	angle_x += ROTATION_SPEED_X / 3 * x;
	angle_y += ROTATION_SPEED_X / 3 * y;
	angle_x = clamp(angle_x, -M_PI / 2, M_PI / 2);
	rotation_axisx = transpose(
		get_rotation_matrix(angle_x, (t_float4){1, 0, 0, 0}));
	rotation_axisy = get_rotation_matrix(angle_y,
		((t_float4 *)&rotation_axisx)[1]);
	return (mul(rotation_axisy, transpose(rotation_axisx)));
}
