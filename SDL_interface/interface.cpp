/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 15:18:25 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/04 00:02:45 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

void	*ife_cleanup_callback(void *ptr)
{
	t_ife	*ife;

	ife = (t_ife *)ptr;
	if (ife->window)
		SDL_DestroyWindow(ife->window);
	SDL_Quit();
	free(ife);
	return (NULL);
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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
		return (ife_cleanup_callback((void *)ife));
	ife->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (ife->window == NULL)
		return (ife_cleanup_callback((void *)ife));

	ife->renderer = SDL_CreateRenderer(ife->window, -1, SDL_RENDERER_ACCELERATED);

	if (TTF_Init() != 0)
		return (ife_cleanup_callback((void *)ife));
	ife->font = TTF_OpenFont("interface_font.ttf", 64);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	/*  */
	SDL_Surface *state = IMG_Load("state.png");
	ife->state = SDL_CreateTextureFromSurface(ife->renderer, state);
	SDL_FreeSurface(state);
	/*  */

	return (ife);
}

int	loop(t_ife *ife)
{
	SDL_Event	evt;
	const Uint8	*state;

	SDL_SetRenderDrawBlendMode(ife->renderer, SDL_BLENDMODE_BLEND);

	char *s[] = {"gl_mesh_0", "gl_mesh_1", "gl_mesh_2", "gl_mesh_3", "gl_mesh_4",
	"gl_mesh_5", "gl_mesh_6", "gl_mesh_7", "gl_mesh_8", "gl_mesh_9", "gl_mesh_10",
	"gl_mesh_11", "gl_mesh_12", "gl_mesh_13", "gl_mesh_14", "gl_mesh_15", "gl_mesh_16", NULL};

	void *widget = widget_init(ife, "Objects", s);

	int is_running = 1;

	while (is_running) {
		SDL_Event evt;
		while (SDL_PollEvent(&evt)) {
			state = SDL_GetKeyboardState(NULL);
			if (state == NULL
				|| evt.type == SDL_QUIT
					|| state[SDL_SCANCODE_ESCAPE])
			{
				is_running = 0;
				break ;
			}
			if (evt.type == SDL_MOUSEWHEEL) {
				int sign = evt.wheel.y > 0 ? -1 : 1;
				widget_scroll(ife, widget, 16 * sign);
			}
			if (evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				widget_change_elem_status(widget, x, y);
			}
			SDL_SetRenderDrawColor(ife->renderer, 0, 0, 0, 255);
			SDL_RenderClear(ife->renderer);
			widget_render(ife, widget + sizeof(uint32_t), *(uint32_t *)widget);
			SDL_RenderPresent(ife->renderer);
		}
	}

	return (0);
}

int	main(void)
{
	t_ife	*ife;

	ife = ife_init(1280, 720, "interface");
	if (ife == NULL)
		return (-1);
	loop(ife);
	ife_cleanup_callback((void *)ife);
	exit (0);
}
