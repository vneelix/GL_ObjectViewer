/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl_project.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:38:39 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 17:05:10 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_PROJECT_H
# define OPENGL_PROJECT_H

# define GL_SILENCE_DEPRECATION

# include <math.h>
# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <OpenGL/gl3.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>

# include "minilib.h"
# include "opengl_init.h"

# define W 1600
# define H 900
# define ROTATION_SPEED_X 0.017453292519943
# define ROTATION_SPEED_Y 0.017453292519943

typedef struct s_ife {
	uint16_t		width;
	uint16_t		height;

	SDL_Window		*window;

	SDL_GLContext	glcontext;
}	t_ife;

typedef struct s_gl_renderer {
	t_opengl	gl;
	float		fov;
	t_uint2		canvas;
	int			w;
	int			h;

	void		**object;
	GLuint		obj_count;
	GLuint		*vtx_array;
	GLuint		*vao_array;
	GLuint		*vbo_array;
	GLuint		*model_data;

	float		k;
	t_float4	axis_offset;
	t_float4	axis_offset_backup;

	uint32_t	flags;
	t_ife		ife;

	uint32_t	rendered_obj;
	long		object_index;
}	t_gl_renderer;

int	gl_renderer_format_model(t_gl_renderer *gl_renderer);
int	gl_model_find_axis_borders(float *vertex, t_float4 *axis_border);
int	gl_renderer_calc_axis_offset(t_gl_renderer *gl_renderer, void **data);
int	gl_uniforms_init(t_gl_renderer *gl_renderer);
int	gl_load_texture(const char *image_path,
		GLuint texture, uint8_t **image, char **err);

int	gl_renderer_release(t_gl_renderer *gl_renderer, int err_code);
int	gl_renderer_init(t_gl_renderer *gl_renderer,
		const char *wavefront_object_path, char **err);
int	ife_cleanup_callback(t_ife *ife, int err_code);
int	ife_init(t_ife *ife, unsigned int width,
		unsigned int height, const char *title);
int	change_color(SDL_Event *evt,
		t_gl_renderer *rndr);
int	event_handler(t_gl_renderer *rndr,
		const Uint8 *state, SDL_Event *evt, uint8_t *flags);
int	renderer(t_gl_renderer *rndr, uint8_t *flags);

#endif
