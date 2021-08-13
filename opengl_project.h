/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl_project.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:38:39 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 03:42:54 by vneelix          ###   ########.fr       */
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
# include <OpenGL/gl3.h>

# include "minilib.h"
# include "opengl_init.h"
# include "GLFW/glfw3.h"

# include "interface.h"


# define W 1280
# define H 800
# define ROTATION_SPEED_X (M_PI / 180.)
# define ROTATION_SPEED_Y (M_PI / 180.)

typedef struct s_gl_renderer {
	t_opengl	gl;
	float		fov;
	t_uint2		canvas;

	void		**object;

	GLuint		obj_count;
	GLuint		*vtx_array;
	GLuint		*vao_array;
	GLuint		*vbo_array;
	GLuint		*model_data;

	t_float4	axis_offset;

	uint32_t	flags;

	t_ife		*ife;
	void		*scroll_widget;
}	t_gl_renderer;

int	gl_renderer_format_model(t_gl_renderer *gl_renderer);
int	gl_model_find_axis_borders(float *vertex, t_float4 *axis_border);
int	gl_renderer_calc_axis_offset(t_gl_renderer *gl_renderer, void **data);
int	gl_uniforms_init(t_gl_renderer *gl_renderer);
int	gl_load_texture(const char *image_path, GLuint texture, char **err);

int	gl_renderer_release(t_gl_renderer *gl_renderer, int err_code);
int	gl_renderer_init(t_gl_renderer *gl_renderer,
		const char *wavefront_object_path, char **err);

#endif
