/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 15:18:43 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/01 16:31:15 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

# include <stdlib.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>
# include <SDL2/SDL_image.h>

typedef struct s_ife {
	__uint16_t		width;
	__uint16_t		height;

	SDL_Window		*window;
	SDL_Renderer	*renderer;

	TTF_Font		*font;
}	t_ife;

#endif
