/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:21:07 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 21:04:48 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_init.h"

static int	gl_uniforms_get(t_opengl *gl)
{
	GLint	err;

	err = 0;
	gl->mode
		= glGetUniformLocation(gl->program, "mode");
	err += glGetError();
	gl->color
		= glGetUniformLocation(gl->program, "color");
	err += glGetError();
	gl->center
		= glGetUniformLocation(gl->program, "center");
	err += glGetError();
	gl->rotation_matrix
		= glGetUniformLocation(gl->program, "rotation");
	err += glGetError();
	gl->projection_matrix = glGetUniformLocation(gl->program, "projection");
	err += glGetError();
	gl->translation_matrix = glGetUniformLocation(gl->program, "translation");
	err += glGetError();
	return (err);
}

int	gl_init(t_opengl *gl, const char *vertex_shader,
			const char *fragment_shader, char **err)
{
	GLint	err_code;

	err_code = 0;
	gl->program = program_get(vertex_shader, fragment_shader, err);
	if (gl->program == 0)
		return (-1);
	if (gl_uniforms_get(gl))
	{
		glDeleteProgram(gl->program);
		return (-1);
	}
	glGenTextures(1, &gl->texture);
	glGenTextures(1, &gl->palette);
	glGenFramebuffers(1, &gl->framebuffer);
	return (0);
}
