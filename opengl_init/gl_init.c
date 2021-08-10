/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:21:07 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/10 23:19:52 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_init.h"

int	gl_init(t_opengl *gl, const char *vertex_shader,
			const char *fragment_shader, char **err)
{
	GLint	err_code;

	err_code = 0;
	gl->program = program_get(vertex_shader, fragment_shader, err);
	if (gl->program == 0)
		return (-1);
	gl->projection_matrix = glGetUniformLocation(gl->program, "projection");
	err_code += glGetError();
	gl->rotation_axisx_matrix
		= glGetUniformLocation(gl->program, "rotation_axisx");
	err_code += glGetError();
	gl->rotation_axisy_matrix
		= glGetUniformLocation(gl->program, "rotation_axisy");
	gl->rotation_axisz_matrix
		= glGetUniformLocation(gl->program, "rotation_axisz");
	err_code += glGetError();
	gl->translation_matrix = glGetUniformLocation(gl->program, "translation");
	err_code += glGetError();
	if (err_code)
	{
		glDeleteProgram(gl->program);
		return (-1);
	}
	return (0);
}
