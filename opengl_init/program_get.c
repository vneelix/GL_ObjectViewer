/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 20:25:41 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/10 20:27:22 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_init.h"

static GLint	program_iv(GLuint program, GLenum pname, char **str)
{
	GLint	ret;
	GLsizei	size;

	glGetProgramiv(program, pname, &ret);
	if (ret != GL_TRUE)
	{
		if (str == NULL)
			return (0);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
		if (size < 0)
			return (0);
		*str = malloc(size);
		if (*str == NULL)
			return (0);
		glGetProgramInfoLog(program, size, NULL, *str);
		return (0);
	}
	return (1);
}

static GLuint	program_load(
				GLuint vertex_shader, GLuint fragment_shader, char **err)
{
	GLuint	program;

	program = glCreateProgram();
	if (program == 0)
		return ((GLuint)error_callback(
				0, "glCreateProgram", "generate program id failure", err));
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	if (program_iv(program, GL_LINK_STATUS, err) == 0)
	{
		glDeleteProgram(program);
		return (0);
	}
	return (program);
}

GLuint	program_get(const char *vertex_shader,
		const char *fragment_shader, char **err)
{
	GLuint	program;
	GLuint	v_shader;
	GLuint	f_shader;

	v_shader = shader_get(vertex_shader, GL_VERTEX_SHADER, err);
	f_shader = shader_get(fragment_shader, GL_FRAGMENT_SHADER, err);
	if (v_shader == 0 || f_shader == 0)
	{
		glDeleteShader(v_shader);
		glDeleteShader(f_shader);
		return (0);
	}
	program = program_load(v_shader, f_shader, err);
	glDeleteShader(v_shader);
	glDeleteShader(f_shader);
	return (program);
}
