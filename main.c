/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 00:35:09 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 05:07:31 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

int	renderer_init(t_gl_renderer *rndr, const char *file, char **err)
{
	rndr->ife = ife_init(rndr->canvas.x, rndr->canvas.y, "3DViewer");
	if (rndr->ife == NULL)
		return (gl_renderer_release(rndr, -1));
	if (SDL_GL_MakeCurrent(rndr->ife->window, rndr->ife->glcontext))
		exit(0);
	if (gl_renderer_init(rndr, file, err))
		return (ife_cleanup_callback(rndr->ife, -1));
	/*SDL_SetRenderDrawBlendMode(rndr->ife->renderer, SDL_BLENDMODE_BLEND);
	((void **)((uint32_t *)rndr->object + 1))[*(uint32_t *)rndr->object] = NULL;
	rndr->scroll_widget = widget_init(rndr->ife, "Objects",
			(void *)rndr->object + sizeof(uint32_t));*/
	glEnable(GL_DEPTH_TEST);
	return (0);
}

int	argv_handling(int argc, char **argv, uint32_t *flags, char **err)
{
	uint32_t	i;

	if (argc == 1)
		return (error_callback(-1, "usage",
				"wavefront_object_file [-fn -ft]", err));
	i = 1;
	while (i != (uint32_t)argc)
	{
		if (!strcmp("-fn", argv[i]))
			*flags |= 1;
		else if (!strcmp("-ft", argv[i]))
			*flags |= 1 << 1;
		i++;
	}
	return (0);
}

int	render_scroll_widger(t_gl_renderer *rndr)
{
	SDL_SetRenderDrawColor(rndr->ife->renderer, 0, 0, 0, 255);
	SDL_RenderClear(rndr->ife->renderer);
	widget_render(rndr->ife, (void *)rndr->scroll_widget
		+ 4, *(uint32_t *)rndr->scroll_widget);
	SDL_RenderPresent(rndr->ife->renderer);
	return (0);
}

int	render_opengl_model(t_gl_renderer *rndr)
{
	uint32_t	i;

	i = 0;
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rndr->gl.texture);
	while (i != rndr->obj_count)
	{
		glBindVertexArray(rndr->vao_array[i]);
		glDrawArrays(GL_TRIANGLES, 0, rndr->vtx_array[i]);
		glBindVertexArray(0);
		i++;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_GL_SwapWindow(rndr->ife->window);
	return (0);
}

int	loop(t_gl_renderer *rndr)
{
	SDL_Event	evt;
	const Uint8	*state;
	uint8_t		mouse_button;

	glUseProgram(rndr->gl.program);
	while (1)
	{
		while (SDL_PollEvent(&evt))
		{
			state = SDL_GetKeyboardState(NULL);
			if (state == NULL
				|| evt.type == SDL_QUIT
				|| state[SDL_SCANCODE_ESCAPE])
				return (0);
			if (evt.type == SDL_MOUSEBUTTONDOWN
				&& evt.button.button == SDL_BUTTON_LEFT)
				mouse_button = mouse_button | (1 << 0);
			if (evt.type == SDL_MOUSEBUTTONUP
				&& evt.button.button == SDL_BUTTON_LEFT)
				mouse_button = mouse_button & ~(1 << 0);
			if (evt.type == SDL_MOUSEBUTTONDOWN
				&& evt.button.button == SDL_BUTTON_RIGHT)
				mouse_button = mouse_button | (1 << 1);
			if (evt.type == SDL_MOUSEBUTTONUP
				&& evt.button.button == SDL_BUTTON_RIGHT)
				mouse_button = mouse_button & ~(1 << 1);
			if (evt.type == SDL_MOUSEMOTION && (mouse_button & 1) == 1)
			{
				t_matrix4x4 m = rotation_manager(evt.motion.yrel, evt.motion.xrel);
				glUniformMatrix4fv(rndr->gl.rotation_matrix, 1, GL_TRUE, &m);
			}
			if (evt.type == SDL_MOUSEMOTION && (mouse_button & 2) == 2)
			{
				rndr->axis_offset += (t_float4){evt.motion.xrel * 1e-2, -evt.motion.yrel * 1e-2, 0, 0};
				translation_matrix(rndr->axis_offset.x, rndr->axis_offset.y, rndr->axis_offset.z, rndr->gl.translation_matrix);
			}
			if (evt.type == SDL_MOUSEWHEEL)
			{
				rndr->axis_offset.z += -evt.wheel.y * 1e-1;
				translation_matrix(rndr->axis_offset.x, rndr->axis_offset.y, rndr->axis_offset.z, rndr->gl.translation_matrix);
			}
			render_opengl_model(rndr);
		}
	}
	glUseProgram(0);
	return (0);
}

int	main(int argc, char **argv)
{
	char			*err;
	t_gl_renderer	rndr;

	bzero(&rndr, sizeof(rndr));
	if (argv_handling(argc, argv, &rndr.flags, &err))
	{
		printf("%s\n", err);
		exit(0);
	}
	rndr.fov = 60 * pi / 180.;
	rndr.canvas = (t_uint2){W, H};
	if (renderer_init(&rndr, argv[1], &err))
	{
		printf("%s\n", err);
		exit(0);
	}
	loop(&rndr);
	return (0);
}
