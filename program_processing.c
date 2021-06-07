#include "opengl_project.h"

GLuint	load_shader(char *path, GLenum shader_type) {
	char *file;
	if (!(uint64_t)(file = ft_getfile(path, 0))) {
		char *message = "An error occurred during read shader\n";
		write(2, message, strlen(message));
		return 0;
	}

	GLuint shader;
	if (!(shader = glCreateShader(shader_type))) {
		char *message = "An error occurred during creation shader\n";
		write(2, message, strlen(message));
		free(file);
		return 0;
	}

	glShaderSource(shader, 1, &file, null);
	free(file);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		GLchar *message;
		GLsizei message_length;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &message_length);
		if ((message = malloc(sizeof(GLchar) * message_length)) == null) {
			glDeleteShader(shader);
			return 0;
		}
		glGetShaderInfoLog(shader, message_length, null, message);
		write(2, message, message_length - 1);
		glDeleteShader(shader);
		free(message);
		return 0;
	}

	return shader;
}

GLuint	create_program() {
	GLuint program, vertex_shader, fragment_shader;

	vertex_shader = load_shader("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	fragment_shader = load_shader("shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	if (!fragment_shader || !vertex_shader) {
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		GLchar *message;
		GLsizei message_length;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &message_length);
		if ((message = malloc(sizeof(GLchar) * message_length)) == null) {
			glDeleteProgram(program);
			return 0;
		}
		glGetProgramInfoLog(program, message_length, null, message);
		write(2, message, message_length - 1);
		glDeleteShader(program);
		free(message);
		return 0;
	}

	return program;
}
