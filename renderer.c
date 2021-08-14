/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 12:28:01 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 12:50:37 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

static int	render_opengl_model(t_gl_renderer *rndr)
{
	uint32_t	i;

	i = 0;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rndr->gl.texture);
	while (i != rndr->rendered_obj)
	{
		glBindVertexArray(rndr->vao_array[i + rndr->object_index]);
		glDrawArrays(GL_TRIANGLES, 0, rndr->vtx_array[i + rndr->object_index]);
		glBindVertexArray(0);
		i++;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return (0);
}

static int	render_framebuffer(GLuint framebuffer,
	t_float4 buffer_rect, t_float4 win_rect)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBlitFramebuffer(buffer_rect.x, buffer_rect.y,
		buffer_rect.z, buffer_rect.w,
		win_rect.x, win_rect.y, win_rect.z, win_rect.w,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	return (glGetError());
}

static int	render_palette(t_gl_renderer *rndr)
{
	render_framebuffer(rndr->gl.framebuffer,
		(t_float4){0, 0, *(uint32_t *)rndr->gl.palette_img,
		*((uint32_t *)rndr->gl.palette_img + 1)},
		(t_float4){0, 0, (int)(rndr->w * 0.3), (int)(rndr->h * 0.3)});
	return (0);
}

int	renderer(t_gl_renderer *rndr, uint8_t *flags)
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render_opengl_model(rndr);
	if ((*flags & 4) == 4)
		render_palette(rndr);
	SDL_GL_SwapWindow(rndr->ife.window);
	return (0);
}
