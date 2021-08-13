/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 15:18:43 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/13 03:17:50 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

# include <stdlib.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>
# include <SDL2/SDL_image.h>

# define WIDGET_WIN_PART (1. / 6.)
# define WIDGET_HEADER_PART (1. / 3.)
# define WIDGET_HEADER_HEIGHT 24
# define TEXT_BOX_PART (1. / 1.2)
# define TEXT_BOX_HEIGHT 28
# define TEXT_BOX_PADDING 16

typedef struct s_ife {
	uint16_t		width;
	uint16_t		height;

	SDL_Window		*window;
	SDL_Renderer	*renderer;

	TTF_Font		*font;

	SDL_Texture		*state;

	SDL_GLContext	glcontext;
}	t_ife;

void	*widget_init(t_ife *ife, const char *title, const char **string);
int		widget_render(t_ife *ife, void **widget, uint32_t widget_elem_count);
int		widget_change_elem_status(void *widget, int x, int y);
int		widget_scroll(t_ife *ife, void *widget, int px);

int		ife_cleanup_callback(t_ife *ife, int err_code);
t_ife	*ife_init(unsigned int width,
		unsigned int height, const char *title);

#endif
