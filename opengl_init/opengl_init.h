/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl_init.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 23:07:33 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 00:33:29 by vneelix          ###   ########.fr       */
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

	GLint	mode;
	GLint	color;
	GLint	center;
	GLint	rotation_matrix;
	GLint	projection_matrix;
	GLint	translation_matrix;

	GLuint	texture;
}	t_opengl;

t_matrix4x4	rotation_manager(int8_t x, int8_t y);

GLuint		shader_get(const char *path, GLenum shader_type, char **err);
GLuint		program_get(const char *vertex_shader,
				const char *fragment_shader, char **err);

t_matrix4x4	get_translation_matrix(double x, double y, double z);
GLint		translation_matrix(double x, double y, double z, GLint uniform);
GLint		perspective_projection_matrix(double fov,
				double aspect, t_float2 clip, GLint uniform);
t_matrix4x4	get_rotation_matrix(double angle, t_float4 axis);
GLint		rotation_matrix(double angle, t_float4 axis, GLint uniform);
GLint		color_init(t_float4 color, GLint uniform);
int			gl_init(t_opengl *gl, const char *vertex_shader,
				const char *fragment_shader, char **err);

#endif
