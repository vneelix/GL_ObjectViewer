/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 19:12:27 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/04 00:01:24 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

static SDL_Texture	*render_text(t_ife *ife,
						const char *text, uint32_t *w, uint32_t *h)
{
	SDL_Surface	*surf;
	SDL_Texture	*textr;

	surf = TTF_RenderText_Blended(ife->font,
			text, (SDL_Color){255, 255, 255, 255});
	if (surf == NULL)
		return (NULL);
	if (w != NULL)
		*w = surf->w;
	if (h != NULL)
		*h = surf->h;
	textr = SDL_CreateTextureFromSurface(ife->renderer, surf);
	SDL_FreeSurface(surf);
	return (textr);
}

static void	*widget_get_memory(const char **string,
				uint32_t *out_widget_elem_count)
{
	void		*ret;
	void		**widget;
	uint32_t	widget_elem_count;

	widget_elem_count = 0;
	while (string[widget_elem_count] != NULL)
		widget_elem_count++;
	widget_elem_count += 2;
	ret = calloc(sizeof(uint32_t) + sizeof(void *) * 4
			+ (sizeof(SDL_Rect) + sizeof(SDL_Color)
				+ sizeof(SDL_Texture *)) * widget_elem_count, 1);
	if (ret == NULL)
		return (NULL);
	*(uint32_t *)ret = widget_elem_count;
	widget = ret + sizeof(uint32_t);
	widget[0] = (void *)widget + sizeof(void *) * 4;
	widget[1] = widget[0] + sizeof(SDL_Rect) * widget_elem_count;
	widget[2] = widget[1] + sizeof(SDL_Color) * widget_elem_count;
	widget[3] = widget[2] + sizeof(SDL_Texture *) * widget_elem_count;
	if (out_widget_elem_count != NULL)
		*out_widget_elem_count = widget_elem_count;
	return (ret);
}

static int	widget_box_init(t_ife *ife, void **widget, const char *title)
{
	SDL_Rect	*widget_box;

	widget = (void *)widget + sizeof(uint32_t);
	((SDL_Rect *)(widget[0]))[0] = (SDL_Rect){ife->width - ife->width
		* WIDGET_WIN_PART, 0, ife->width * WIDGET_WIN_PART + 4, ife->height};
	((SDL_Color *)(widget[1]))[0] = (SDL_Color){0, 0, 0, 0};
	if (ife->state == NULL)
		((SDL_Color *)(widget[1]))[0] = (SDL_Color){148, 148, 148, 255};
	((SDL_Texture **)(widget[2]))[0] = ife->state;
	widget_box = (SDL_Rect *)(widget[0]);
	((SDL_Rect *)(widget[0]))[1] = (SDL_Rect){widget_box->x
		+ (widget_box->w - widget_box->w * WIDGET_HEADER_PART)
		/ 2., widget_box->h * 0.016,
		widget_box->w * WIDGET_HEADER_PART + 4, WIDGET_HEADER_HEIGHT};
	((SDL_Color *)(widget[1]))[1] = (SDL_Color){0, 0, 0, 0};
	((SDL_Texture **)(widget[2]))[1] = render_text(ife, title, NULL, NULL);
	return (0);
}

static int	widget_text_boxs_init(t_ife *ife,
				void **widget, uint32_t widget_elem_count, const char **string)
{
	uint32_t	i;
	SDL_Rect	*rect_writer;
	SDL_Color	*color_writer;
	SDL_Texture	**texture_writer;

	i = 2;
	rect_writer = widget[0];
	color_writer = widget[1];
	texture_writer = widget[2];
	while (i != widget_elem_count)
	{
		color_writer[i] = (SDL_Color){255, 0, 0, 16};
		texture_writer[i] = render_text(ife, string[i - 2], NULL, NULL);
		rect_writer[i] = (SDL_Rect){
			rect_writer[0].x + (rect_writer[0].w
				- rect_writer[0].w * TEXT_BOX_PART) / 2.,
			rect_writer[i - 1].y + rect_writer[i - 1].h + TEXT_BOX_PADDING,
			rect_writer[0].w * TEXT_BOX_PART,
			TEXT_BOX_HEIGHT
		};
		i++;
	}
	return (0);
}	

void	*widget_init(t_ife *ife, const char *title, const char **string)
{
	void		**widget;
	uint32_t	widget_elem_count;

	widget = (void **)widget_get_memory(string, &widget_elem_count);
	if (widget == NULL)
		return (NULL);
	widget_box_init(ife, widget, title);
	widget_text_boxs_init(ife, (void *)widget
		+ sizeof(uint32_t), *(uint32_t *)widget, string);
	return (widget);
}
