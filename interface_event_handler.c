/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_event_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 12:22:13 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 12:24:29 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

static int	mouse_button_handler(SDL_Event *evt,
	uint8_t *flags, t_gl_renderer *rndr)
{
	if ((*flags & 4) == 4)
	{
		change_color(evt, rndr);
		return (0);
	}
	if ((evt->type == SDL_MOUSEBUTTONDOWN
			|| evt->type == SDL_MOUSEBUTTONUP)
		&& evt->button.button == SDL_BUTTON_LEFT)
		*flags ^= 1;
	if ((evt->type == SDL_MOUSEBUTTONDOWN
			|| evt->type == SDL_MOUSEBUTTONUP)
		&& evt->button.button == SDL_BUTTON_RIGHT)
		*flags ^= 2;
	return (0);
}

static int	mouse_motion_handler(SDL_Event *evt,
	uint8_t *flags, t_gl_renderer *rndr)
{
	float		k;
	t_matrix4x4	m;

	if (evt->type == SDL_MOUSEMOTION && (*flags & 1) == 1)
	{
		m = rotation_manager(evt->motion.yrel, evt->motion.xrel);
		glUniformMatrix4fv(rndr->gl.rotation_matrix,
			1, GL_TRUE, (const GLfloat *)&m);
	}
	else if (evt->type == SDL_MOUSEMOTION && (*flags & 2) == 2)
	{
		k = clamp(
				1e-2 * fabs(rndr->axis_offset.z / 4.) * 0.75, 0.004, INFINITY);
		rndr->axis_offset += (t_float4){
			evt->motion.xrel * k, -evt->motion.yrel * k, 0, 0};
		translation_matrix(rndr->axis_offset.x, rndr->axis_offset.y,
			rndr->axis_offset.z, rndr->gl.translation_matrix);
	}
	return (0);
}

static int	mouse_wheel_handler(SDL_Event *evt,
	uint8_t *flags, t_gl_renderer *rndr)
{
	flags = NULL;
	rndr->axis_offset.z += -evt->wheel.y * 0.1 * rndr->k;
	translation_matrix(rndr->axis_offset.x, rndr->axis_offset.y,
		rndr->axis_offset.z, rndr->gl.translation_matrix);
	return (0);
}

static int	keyboard_handler(t_gl_renderer *rndr,
	const Uint8 *state, SDL_Event *evt, uint8_t *flags)
{
	if (evt->type == SDL_KEYDOWN && state[SDL_SCANCODE_C])
		*flags ^= 4;
	else if (evt->type == SDL_KEYDOWN && state[SDL_SCANCODE_SPACE])
	{
		*flags ^= 8;
		glUniform1f(rndr->gl.mode, (*flags & 8) >> 3);
	}
	else if (evt->type == SDL_KEYDOWN && state[SDL_SCANCODE_R])
	{
		translation_matrix(rndr->axis_offset_backup.x,
			rndr->axis_offset_backup.y,
			rndr->axis_offset_backup.z, rndr->gl.translation_matrix);
		rndr->axis_offset = rndr->axis_offset_backup;
	}
	return (0);
}

int	event_handler(t_gl_renderer *rndr,
	const Uint8 *state, SDL_Event *evt, uint8_t *flags)
{
	if (evt->type == SDL_KEYDOWN)
		keyboard_handler(rndr, state, evt, flags);
	else if (evt->type == SDL_MOUSEBUTTONDOWN
		|| evt->type == SDL_MOUSEBUTTONUP)
		mouse_button_handler(evt, flags, rndr);
	else if (evt->type == SDL_MOUSEMOTION)
		mouse_motion_handler(evt, flags, rndr);
	else if (evt->type == SDL_MOUSEWHEEL)
		mouse_wheel_handler(evt, flags, rndr);
	return (0);
}
