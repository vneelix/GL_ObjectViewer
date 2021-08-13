/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 00:40:14 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 05:19:04 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

int	ife_cleanup_callback(t_ife *ife, int err_code)
{
	if (ife->renderer != NULL)
		SDL_DestroyRenderer(ife->renderer);
	if (ife->window != NULL)
		SDL_DestroyWindow(ife->window);
	if (ife->font != NULL)
		TTF_CloseFont(ife->font);
	if (ife->state != NULL)
		SDL_DestroyTexture(ife->state);
	if (ife->glcontext != NULL)
		SDL_GL_DeleteContext(ife->glcontext);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	free(ife);
	return (err_code);
}

SDL_Texture	*load_texture_from_img(const char *image_path, SDL_Renderer *rndr)
{
	SDL_Surface	*sfe;
	SDL_Surface	*ptr;
	SDL_Texture	*txe;

	if (rndr == NULL)
		return (NULL);
	sfe = IMG_Load(image_path);
	if (sfe == NULL)
		return (NULL);
	ptr = SDL_ConvertSurfaceFormat(sfe, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(sfe);
	if (ptr == NULL)
		return (NULL);
	txe = SDL_CreateTextureFromSurface(rndr, ptr);
	SDL_FreeSurface(ptr);
	return (txe);
}

static int	sdl_opengl_configure(void)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	return (0);
}

t_ife	*ife_init(unsigned int width,
		unsigned int height, const char *title)
{
	t_ife	*ife;

	ife = calloc(1, sizeof(t_ife));
	if (ife == NULL)
		return (NULL);
	ife->width = width;
	ife->height = height;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0
		|| IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0 || TTF_Init() != 0
		|| sdl_opengl_configure())
		return ((void *)(uint64_t)ife_cleanup_callback(ife, 0));
	ife->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, width, height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (ife->window != NULL)
		ife->glcontext = SDL_GL_CreateContext(ife->window);
	/*ife->renderer
		= SDL_CreateRenderer(ife->window, -1, SDL_RENDERER_ACCELERATED);
	ife->font = TTF_OpenFont("resources/interface_font.ttf", 64);
	ife->state = load_texture_from_img("resources/state.png", ife->renderer);
	if (ife->font == NULL || ife->state == NULL || ife->glcontext == NULL
		|| ife->window == NULL || ife->renderer == NULL)
		return ((void *)(uint64_t)ife_cleanup_callback(ife, 0));*/
	return (ife);
}
