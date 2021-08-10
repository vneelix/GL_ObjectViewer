/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 19:37:01 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/10 19:37:59 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_init.h"

static GLint	shader_iv(GLuint shader, GLenum pname, char **str)
{
	GLint	ret;
	GLsizei	size;

	glGetShaderiv(shader, pname, &ret);
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

static GLuint	shader_load(
				const char *shader_source, GLenum shader_type, char **err)
{
	GLuint	shader;

	if (shader_source == NULL)
		return ((GLuint)error_callback(
				0, "shader", "missing string with shader source", err));
	shader = glCreateShader(shader_type);
	if (shader == 0)
		return ((GLuint)error_callback(
				0, "glCreateShader", "generate shader id failure", err));
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);
	if (shader_iv(shader, GL_COMPILE_STATUS, err) == 0)
	{
		glDeleteShader(shader);
		return (0);
	}
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
