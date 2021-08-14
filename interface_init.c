/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 00:40:14 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 20:05:11 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

static int	sdl_opengl_configure(void)
{
	return (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE)
		|| SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)
		|| SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1)
		|| SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8)
		|| SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8)
		|| SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8)
		|| SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8)
		|| SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
}

int	ife_cleanup_callback(t_ife *ife, int err_code)
{
	if (ife->window != NULL)
		SDL_DestroyWindow(ife->window);
	if (ife->glcontext != NULL)
		SDL_GL_DeleteContext(ife->glcontext);
	IMG_Quit();
	SDL_Quit();
	return (err_code);
}

int	ife_init(t_ife *ife, unsigned int width,
		unsigned int height, const char *title)
{
	if (ife == NULL)
		return (-1);
	ife->width = width;
	ife->height = height;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0
		|| IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0 || sdl_opengl_configure())
		return (ife_cleanup_callback(ife, -1));
	ife->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, width, height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	if (ife->window == NULL)
		return (ife_cleanup_callback(ife, -1));
	ife->glcontext = SDL_GL_CreateContext(ife->window);
	if (ife->glcontext == NULL)
		return (ife_cleanup_callback(ife, -1));
	return (0);
}
