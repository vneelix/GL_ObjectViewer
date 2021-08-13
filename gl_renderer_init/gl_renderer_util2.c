/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_renderer_util2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 22:59:52 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/12 23:11:57 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

uint8_t	*load_image(const char *image_path)
{
	SDL_Surface	*sfe;
	SDL_Surface	*ptr;
	uint8_t		*img;

	sfe = IMG_Load(image_path);
	if (sfe == NULL)
		return (NULL);
	ptr = SDL_ConvertSurfaceFormat(sfe, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(sfe);
	img = calloc(ptr->w * ptr->pitch + 8, sizeof(uint8_t));
	if (img == NULL)
	{
		SDL_FreeSurface(ptr);
		return (NULL);
	}
	*(uint32_t *)img = ptr->w;
	*((uint32_t *)img + 1) = ptr->h;
	memcpy(img + 8, ptr->pixels, ptr->w * ptr->pitch * sizeof(uint8_t));
	SDL_FreeSurface(ptr);
	return (img);
}

int	gl_load_texture(const char *image_path, GLuint texture, char **err)
{
	uint8_t	*img;

	img = load_image(image_path);
	if (img == NULL)
		return (error_callback(-1, image_path, "unable to load image", err));
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *(uint32_t *)img,
		*((uint32_t *)img + 1), 0, GL_RGBA, GL_UNSIGNED_BYTE, img + 8);
	if (glGetError())
	{
		free(img);
		return (error_callback(-1, image_path,
				"unable to load image to gl texture", err));
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	free(img);
	return (0);
}
