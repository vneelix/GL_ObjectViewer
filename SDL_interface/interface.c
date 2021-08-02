/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 15:18:25 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/02 23:48:17 by vneelix          ###   ########.fr       */
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
	return (ife);
}

SDL_Texture	*render_text(t_ife *ife, const char *text, uint32_t *w, uint32_t *h) {
	SDL_Surface	*surf = TTF_RenderText_Blended(ife->font, text, (SDL_Color){255, 255, 255, 255});
	if (surf == NULL)
		return (NULL);
	if (w != NULL)
		*w = surf->w;
	if (h != NULL)
		*h = surf->h;
	SDL_Texture *textr = SDL_CreateTextureFromSurface(ife->renderer, surf);
	SDL_FreeSurface(surf);
	return (textr);
}

void	*widget_init(t_ife *ife, const char *title, const char **string) {
	
	uint32_t	string_count = 0;
	for (; string[string_count]; string_count++);

	void	*widget = calloc(sizeof(uint32_t) + (sizeof(SDL_Texture *) + sizeof(SDL_Rect) + sizeof(SDL_Color)) * (2 + string_count), 1);

	void *ret = widget;

	*(uint32_t *)widget = 2 + string_count;
	widget += sizeof(uint32_t);

	/* const */
	double		widget_win_part = 1. / 6.;
	double		widget_header_part = 1. / 3.;
	uint32_t	widget_header_height = 24;

	double		string_box_part = 1. / 1.5;
	uint32_t	string_box_height = 28;

	uint32_t	string_box_padding = 16;
	/*	*/

	SDL_Texture **texture_writer = widget;
	SDL_Rect	*rect_writer = widget + sizeof(SDL_Texture *) * (2 + string_count);
	SDL_Color	*color_writer = (void *)rect_writer + sizeof(SDL_Rect) * (2 + string_count);

	/* widget write */
	texture_writer++;
	*color_writer++ = (SDL_Color){68, 68, 68, 255};
	*rect_writer++ = (SDL_Rect){ife->width - ife->width * widget_win_part,
										0, ife->width * widget_win_part, ife->height};
	/*	*/

	/* widget header write */
	SDL_Rect *widg = (SDL_Rect *)(rect_writer - 1);

	*texture_writer++ = render_text(ife, title, NULL, NULL);
	*color_writer++ = (SDL_Color){0, 0, 0, 32};
	*rect_writer++ = (SDL_Rect){widg->x + (widg->w - widg->w * widget_header_part) / 2.,
										0, widg->w * widget_header_part, widget_header_height};
	/*  */

	for (uint32_t i = 0; i != string_count; i++) {
		uint32_t w, h;
		//*texture_writer++ = render_text(ife, string[i]);
		*color_writer++ = (SDL_Color){255, 0, 0, 32};
		*texture_writer++ = render_text(ife, string[i], &w, &h);
		*rect_writer = (SDL_Rect){widg->x + (widg->w - widg->w * string_box_part) / 2., rect_writer[-1].y + rect_writer[-1].h + string_box_padding, widg->w * string_box_part, string_box_height};
		rect_writer++;
	}

	return (ret);
}

int	render_widget(t_ife *ife, void *widget) {
	uint32_t i = 0;
	uint32_t widget_elem_count = *(uint32_t *)widget;
	widget += sizeof(uint32_t);

	const SDL_Texture	**texture_reader = widget;
	const SDL_Rect		*rect_reader = (const void *)texture_reader + sizeof(SDL_Texture *) * widget_elem_count;
	const SDL_Color		*color_reader = (const void *)rect_reader + sizeof(SDL_Rect) * widget_elem_count;

	const SDL_Rect *header = rect_reader + 1;

	while (i != widget_elem_count) {
		if (i >= 2 && (rect_reader[i].y < (header->y + header->h))) {
			i++;
			continue;
		}
		SDL_SetRenderDrawColor(ife->renderer, (color_reader + i)->r, (color_reader + i)->g, (color_reader + i)->b, (color_reader + i)->a);
		SDL_RenderFillRect(ife->renderer, rect_reader + i);
		SDL_RenderCopy(ife->renderer, texture_reader[i], NULL, rect_reader + i);
		i++;
	}
	return (0);
}

int	widget_scroll(t_ife *ife, void *widget, int px) {
	uint32_t i = 0;
	uint32_t widget_elem_count = *(uint32_t *)widget;
	widget += sizeof(uint32_t);

	const SDL_Texture	**texture_reader = widget;
	SDL_Rect			*rect_writer = (void *)texture_reader + sizeof(SDL_Texture *) * widget_elem_count;
	const SDL_Color		*color_reader = (const void *)rect_writer + sizeof(SDL_Rect) * widget_elem_count;

	rect_writer += 2;

	SDL_Rect *header = rect_writer - 1;
	SDL_Rect *last_box = rect_writer + widget_elem_count - 2 - 1;

	if (px < 0) {
		if (!(ife->height <= (last_box->y + last_box->h)))
			return (0);
	}

	if (px > 0) {
		if (!((header->y + header->h) >= rect_writer->y))
			return (0);
	}

	while (i != widget_elem_count - 2) {
		rect_writer[i].y += px;
		i++;
	}
	return (0);
}

int	widget_change_elem_status(t_ife *ife, void *widget, uint32_t x, uint32_t y) {
	uint32_t	widget_elem_count = *(uint32_t *)widget;
	const SDL_Rect *rect_reader = widget + sizeof(uint32_t) + sizeof(SDL_Texture *) * widget_elem_count;
	if (x > rect_reader->x && x < (rect_reader->x + rect_reader->w))
	{
		uint32_t	i = 0;

		SDL_Color	*color_writer = (const void *)rect_reader + sizeof(SDL_Rect) * widget_elem_count;

		rect_reader += 2;
		color_writer += 2;

		while (i != widget_elem_count - 2) {
			if ((x >= rect_reader[i].x && x <= (rect_reader[i].x + rect_reader[i].w))
				&& (y >= rect_reader[i].y && y <= (rect_reader[i].y + rect_reader[i].h)))
			{
				if (color_writer[i].r != 255)
					color_writer[i] = (SDL_Color){255, 0, 0, 32};
				else
					color_writer[i] = (SDL_Color){0, 255, 0, 32};
				break ;
			}
			i++;
		}
	}
	return (0);
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
				widget_change_elem_status(ife, widget, x, y);
			}
			SDL_SetRenderDrawColor(ife->renderer, 0, 0, 0, 255);
			SDL_RenderClear(ife->renderer);
			render_widget(ife, widget);
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
