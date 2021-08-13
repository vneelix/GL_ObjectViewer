/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_renderer_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:51:16 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 00:29:03 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

int	gl_renderer_format_model(t_gl_renderer *gl_renderer)
{
	gl_renderer->obj_count = *gl_renderer->model_data;
	gl_renderer->vtx_array = gl_renderer->model_data + 1;
	gl_renderer->vao_array = gl_renderer->vtx_array + gl_renderer->obj_count;
	gl_renderer->vbo_array = gl_renderer->vao_array + gl_renderer->obj_count;
	return (0);
}

int	gl_model_find_axis_borders(float *vertex, t_float4 *axis_border)
{
	uint32_t	i;
	uint32_t	vertex_count;

	i = 0;
	vertex_count = *vertex;
	vertex++;
	bzero(axis_border, sizeof(t_float4) * 6);
	while (i != vertex_count)
	{
		if (vertex[i * 4] > axis_border[0].x)
			memcpy(axis_border + 0, vertex + i * 4, sizeof(t_float4));
		if (vertex[i * 4] < axis_border[1].x)
			memcpy(axis_border + 1, vertex + i * 4, sizeof(t_float4));
		if (vertex[i * 4 + 1] > axis_border[2].y)
			memcpy(axis_border + 2, vertex + i * 4, sizeof(t_float4));
		if (vertex[i * 4 + 1] < axis_border[3].y)
			memcpy(axis_border + 3, vertex + i * 4, sizeof(t_float4));
		if (vertex[i * 4 + 2] > axis_border[4].z)
			memcpy(axis_border + 4, vertex + i * 4, sizeof(t_float4));
		if (vertex[i * 4 + 2] < axis_border[5].z)
			memcpy(axis_border + 5, vertex + i * 4, sizeof(t_float4));
		i++;
	}
	return (0);
}

int	gl_renderer_calc_axis_offset(t_gl_renderer *gl_renderer, void **data)
{
	t_float4	center;
	t_float4	axis_border[6];

	gl_model_find_axis_borders(data[v], axis_border);
	gl_renderer->axis_offset = (t_float4){
		-((axis_border[0].x - axis_border[1].x) / 2 + axis_border[1].x),
		-((axis_border[2].y - axis_border[3].y) / 2 + axis_border[3].y),
		-4 - (axis_border[4].z - axis_border[5].z),
		0
	};
	center = (t_float4){
		axis_border[1].x + (axis_border[0].x - axis_border[1].x) / 2,
		axis_border[3].y + (axis_border[2].y - axis_border[3].y) / 2,
		axis_border[5].z + (axis_border[4].z - axis_border[5].z) / 2,
		1
	};
	glUseProgram(gl_renderer->gl.program);
	glUniform4fv(gl_renderer->gl.center, 1, (const GLfloat *)&center);
	glUseProgram(0);
	return (0);
}

int	gl_uniforms_init(t_gl_renderer *gl_renderer)
{
	glUseProgram(gl_renderer->gl.program);
	rotation_matrix(0, (t_float4){1, 0, 0, 0}, gl_renderer->gl.rotation_matrix);
	translation_matrix(gl_renderer->axis_offset.x, gl_renderer->axis_offset.y,
		gl_renderer->axis_offset.z, gl_renderer->gl.translation_matrix);
	perspective_projection_matrix(gl_renderer->fov,
		(double)gl_renderer->canvas.x / gl_renderer->canvas.y,
		(t_float2){0.128, 512}, gl_renderer->gl.projection_matrix);
	color_init((t_float4){1, 1, 1, 1}, gl_renderer->gl.color);
	glUniform1f(gl_renderer->gl.mode, 0);
	glUseProgram(0);
	return (0);
}
