/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 23:07:16 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/10 00:59:42 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_init.h"

GLint	shader_iv(GLuint shader, GLenum pname, char **str)
{
	GLint	ret;
	GLsizei	size;

	int r = 0;

	r = glGetError();

	glGetShaderiv(shader, pname, &ret);
	r = glGetError();
	if (ret != GL_TRUE)
	{
		if (str == NULL)
			return (0);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
		if (size < 0)
			return (0);
		*str = malloc(size);
		if (*str == NULL)
			return (0);
		glGetShaderInfoLog(shader, size, NULL, *str);
		return (0);
	}
	return (1);
}

GLuint	shader_load(
		const char *shader_source, GLenum shader_type, char **err)
{
	GLuint	shader;

	int r = 0;

	r = glGetError();

	if (shader_source == NULL)
		return ((GLuint)error_callback(
				0, "shader", "missing string with shader source", err));
	shader = glCreateShader(shader_type);
	r = glGetError();
	if (shader == 0)
		return ((GLuint)error_callback(
				0, "glCreateShader", "generate shader id failure", err));
	glShaderSource(shader, 1, &shader_source, NULL);
	r = glGetError();
	glCompileShader(shader);
	r = glGetError();
	if (shader_iv(shader, GL_COMPILE_STATUS, err) == 0)
	{
		glDeleteShader(shader);
		return (0);
	}
	r = glGetError();
	return (shader);
}

GLuint	shader_get(const char *path, GLenum shader_type, char **err)
{
	GLuint	shader;
	char	*shader_source;

	shader_source = ft_read_file_fstat(path);
	if (shader_source == NULL)
		return ((GLuint)error_callback(
				0, path, "failed to read file", err));
	shader = shader_load(shader_source, shader_type, err);
	free(shader_source);
	return (shader);
}

GLint	program_iv(GLuint program, GLenum pname, char **str)
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

GLuint	program_load(GLuint vertex_shader, GLuint fragment_shader, char **err)
{
	GLuint	program;

	int r = 0;

	r = glGetError();
	program = glCreateProgram();
	if (program == 0)
		return ((GLuint)error_callback(
				0, "glCreateProgram", "generate program id failure", err));
	r = glGetError();
	glAttachShader(program, vertex_shader);
	r = glGetError();
	glAttachShader(program, fragment_shader);
	r = glGetError();
	glLinkProgram(program);
	r = glGetError();
	if (program_iv(program, GL_LINK_STATUS, err) == 0)
	{
		glDeleteProgram(program);
		return (0);
	}
	r = glGetError();
	return (program);
}

GLuint	program_get(const char *vertex_shader,
		const char *fragment_shader, char **err)
{
	GLuint	program;
	GLuint	v_shader;
	GLuint	f_shader;

	int r = 0;

	v_shader = shader_get(vertex_shader, GL_VERTEX_SHADER, err);
	f_shader = shader_get(fragment_shader, GL_FRAGMENT_SHADER, err);
	if (v_shader == 0 || f_shader == 0)
	{
		glDeleteShader(v_shader);
		glDeleteShader(f_shader);
		return (0);
	}
	program = program_load(v_shader, f_shader, err);
	r = glGetError();
	glDeleteShader(v_shader);
	r = glGetError();
	glDeleteShader(f_shader);
	r = glGetError();
	return (program);
}
