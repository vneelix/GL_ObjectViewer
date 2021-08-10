/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 19:52:47 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/10 23:21:13 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_init.h"

GLint	translation_matrix(double x, double y, double z, GLuint uniform)
{
	t_matrix4x4	matrix;

	matrix = (t_matrix4x4){
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	};
	glUniformMatrix4fv(uniform, 1, GL_TRUE, (const GLfloat *)&matrix);
	return (glGetError());
}

GLint	rotation_matrix(double angle, t_float4 axis, GLuint uniform)
{
	double		s;
	double		c;
	t_matrix4x4	matrix;

	s = sin(angle);
	c = cos(angle);
	matrix = (t_matrix4x4){
		axis.x * axis.x * (1 - c) + c, axis.x * axis.y
		* (1 - c) - axis.z * s, axis.x * axis.z * (1 - c) + axis.y * s, 0,
		axis.y * axis.x * (1 - c) + axis.z * s, axis.y * axis.y
		* (1 - c) + c, axis.y * axis.z * (1 - c) - axis.x * s, 0,
		axis.x * axis.z * (1 - c) - axis.y * s, axis.y * axis.z
		* (1 - c) + axis.x * s, axis.z * axis.z * (1 - c) + c, 0,
		0, 0, 0, 1
	};
	glUniformMatrix4fv(uniform, 1, GL_TRUE, (const GLfloat *)&matrix);
	return (glGetError());
}

GLint	perspective_projection_matrix(double fov,
		double aspect, t_float2 clip, GLuint uniform)
{
	t_matrix4x4	matrix;
	double		cot_half_fov;

	cot_half_fov = 1. / tan(fov / 2.);
	matrix = (t_matrix4x4){
		cot_half_fov / aspect, 0, 0, 0,
		0, cot_half_fov, 0, 0,
		0, 0, (clip[0] + clip[1]) / (clip[0] - clip[1]),
		2 * clip[0] * clip[1] / (clip[0] - clip[1]),
		0, 0, -1, 0
	};
	glUniformMatrix4fv(uniform, 1, GL_TRUE, (const GLfloat *)&matrix);
	return (glGetError());
}
