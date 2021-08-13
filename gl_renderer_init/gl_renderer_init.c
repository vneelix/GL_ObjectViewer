/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_renderer_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 22:22:00 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 02:15:19 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

int	gl_renderer_release(t_gl_renderer *gl_renderer, int err_code)
{
	if (gl_renderer->gl.program != 0)
		glDeleteProgram(gl_renderer->gl.program);
	glDeleteBuffers(gl_renderer->obj_count, gl_renderer->vbo_array);
	glDeleteVertexArrays(gl_renderer->obj_count, gl_renderer->vao_array);
	if (gl_renderer->model_data != NULL)
		free(gl_renderer->model_data);
	if (gl_renderer->object != NULL)
		free(gl_renderer->object);
	return (err_code);
}

int	gl_flags_handling(void **data, uint32_t flags)
{
	if ((flags & (1 << 0)) == (1 << 0))
		*(float *)(data[vn]) = 0;
	if ((flags & (1 << 0)) == (1 << 0))
		*(float *)(data[vt]) = 0;
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
	glUseProgram(gl_renderer->gl.program);
	if (gl_load_texture("image.jpeg", gl_renderer->gl.texture, err))
		return (gl_renderer_release(gl_renderer, -1));
	glUseProgram(0);
	return (0);
}
