/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_renderer_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 22:22:00 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 13:40:47 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

int	gl_renderer_release(t_gl_renderer *gl_renderer, int err_code)
{
	if (gl_renderer->gl.program != 0)
		glDeleteProgram(gl_renderer->gl.program);
	glDeleteBuffers(gl_renderer->obj_count, gl_renderer->vbo_array);
	glDeleteVertexArrays(gl_renderer->obj_count, gl_renderer->vao_array);
	glDeleteTextures(1, &gl_renderer->gl.program);
	glDeleteTextures(1, &gl_renderer->gl.palette);
	if (gl_renderer->model_data != NULL)
		free(gl_renderer->model_data);
	if (gl_renderer->object != NULL)
		free(gl_renderer->object);
	if (gl_renderer->gl.palette_img != NULL)
		free(gl_renderer->gl.palette_img);
	return (err_code);
}

int	gl_flags_handling(void **data, uint32_t flags)
{
	if ((flags & (1 << 0)) == (1 << 0))
		*(float *)(data[vn]) = 0;
	if ((flags & (1 << 1)) == (1 << 1))
		*(float *)(data[vt]) = 0;
	return (0);
}

int	gl_load_textures(t_gl_renderer *gl_renderer, char **err)
{
	glUseProgram(gl_renderer->gl.program);
	if (gl_load_texture("resources/image.jpeg",
			gl_renderer->gl.texture, NULL, err))
	{
		glUseProgram(0);
		return (-1);
	}
	if (gl_load_texture("resources/palette.png",
			gl_renderer->gl.palette, &gl_renderer->gl.palette_img, err))
	{
		glUseProgram(0);
		return (-1);
	}
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gl_renderer->gl.framebuffer);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, gl_renderer->gl.palette, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	if (glGetError())
	{
		glUseProgram(0);
		return (-1);
	}
	glUseProgram(0);
	return (0);
}

int	gl_renderer_init(t_gl_renderer *gl_renderer,
	const char *wavefront_object_path, char **err)
{
	void	**data;
	void	**object;

	if (gl_init(&gl_renderer->gl,
			"shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", err))
		return (-1);
	data = wavefront_object_reader(
			wavefront_object_path, (void **)&object, NULL, err);
	if (data == NULL)
		return (gl_renderer_release(gl_renderer, -1));
	gl_flags_handling(data, gl_renderer->flags);
	gl_renderer->model_data
		= wavefront_to_gl_arrays_converter(data, object, err);
	gl_renderer->object = object;
	if (gl_renderer->model_data == NULL)
		return (gl_renderer_release(gl_renderer, -1));
	gl_renderer_format_model(gl_renderer);
	gl_renderer_calc_axis_offset(gl_renderer, data);
	gl_uniforms_init(gl_renderer);
	free(data);
	if (gl_load_textures(gl_renderer, err))
		return (gl_renderer_release(gl_renderer, -1));
	return (0);
}
