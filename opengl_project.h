/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl_project.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:38:39 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/11 01:34:42 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_PROJECT_H
# define OPENGL_PROJECT_H

# define pi (double)3.14159265358979323846

# define GL_SILENCE_DEPRECATION

# include <math.h>
# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>

# include "minilib.h"
# include "opengl_init.h"
# include <OpenGL/gl3.h>
# include "GLFW/glfw3.h"

typedef struct s_gl_renderer {
	t_opengl	gl;
	float		fov;
	t_uint2		canvas;

	GLuint		obj_count;
	GLuint		*vtx_array;
	GLuint		*vao_array;
	GLuint		*vbo_array;
	GLuint		*model_data;

	t_float4	axis_offset;
	t_float4	axis_angles;
}	t_gl_renderer;

int	gl_renderer_release(t_gl_renderer *gl_renderer, int err_code);
int	gl_renderer_init(t_gl_renderer *gl_renderer,
		const char *wavefront_object_path, char **err);

#endif
