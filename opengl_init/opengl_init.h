/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl_init.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 23:07:33 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/11 01:34:33 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_INIT_H
# define OPENGL_INIT_H
# include "minilib.h"
# include "triangulator.h"
# include "wavefront_object_reader.h"

typedef struct s_opengl
{
	GLuint	program;

	GLuint	translation_matrix;
	GLuint	rotation_axisx_matrix;
	GLuint	rotation_axisy_matrix;
	GLuint	rotation_axisz_matrix;
	GLuint	projection_matrix;
}	t_opengl;

GLuint	shader_get(const char *path, GLenum shader_type, char **err);
GLuint	program_get(const char *vertex_shader,
			const char *fragment_shader, char **err);

GLint	translation_matrix(double x, double y, double z, GLuint uniform);
GLint	perspective_projection_matrix(double fov,
			double aspect, t_float2 clip, GLuint uniform);
GLint	rotation_matrix(double angle, t_float4 axis, GLuint uniform);
int		gl_init(t_opengl *gl, const char *vertex_shader,
			const char *fragment_shader, char **err);

#endif
