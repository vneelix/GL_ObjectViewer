/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_color_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 12:24:55 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 12:25:22 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

static t_float4	calc_delta(t_gl_renderer *rndr)
{
	t_float4	delta;

	delta = (t_float4){
		(double)rndr->w / rndr->ife.width,
		(double)rndr->h / rndr->ife.height,
		(rndr->w * 0.3) / *(uint32_t *)rndr->gl.palette_img,
		(rndr->h * 0.3) / *((uint32_t *)rndr->gl.palette_img + 1)
	};
	return (delta);
}

int	change_color(SDL_Event *evt,
	t_gl_renderer *rndr)
{
	int				x;
	int				y;
	t_float4		color;
	t_float4		delta;
	uint8_t			*ptr;

	delta = calc_delta(rndr);
	x = clamp(evt->motion.x * delta.x, 0, rndr->w);
	y = clamp((H - evt->motion.y) * delta.y, 0, rndr->h);
	if (!((x > 0 && x < (int)(rndr->w * 0.3))
		&& (y > 0 && y < (int)(rndr->h * 0.3))))
		return (0);
	x = clamp(x / delta.z, 0, *(uint32_t *)rndr->gl.palette_img - 1);
	y = clamp(y / delta.w, 0, *(uint32_t *)(rndr->gl.palette_img + 1) - 1);
	ptr = (uint8_t *)((uint32_t *)rndr->gl.palette_img + 2
			+ x + *(uint32_t *)rndr->gl.palette_img * y);
	color.r = ptr[0] / 255.;
	color.g = ptr[1] / 255.;
	color.b = ptr[2] / 255.;
	color.a = ptr[3] / 255.;
	glUniform4fv(rndr->gl.color, 1, (const GLfloat *)&color);
	return (0);
}
