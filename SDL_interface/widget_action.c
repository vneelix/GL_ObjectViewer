/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   widget_action.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 00:00:48 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/04 00:00:58 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

int	widget_render(t_ife *ife, void **widget, uint32_t widget_elem_count)
{
	uint32_t			i;
	const SDL_Rect		*rect_reader;
	const SDL_Color		*color_reader;
	const SDL_Texture	**texture_reader;

	i = 0;
	rect_reader = widget[0];
	color_reader = widget[1];
	texture_reader = widget[2];
	while (i != widget_elem_count)
	{
		if (i >= 2 && (rect_reader[i].y - TEXT_BOX_PADDING
				< (rect_reader[1].y + rect_reader[1].h)))
		{
			i++;
			continue ;
		}
		SDL_SetRenderDrawColor(ife->renderer, color_reader[i].r,
			color_reader[i].g, color_reader[i].b, color_reader[i].a);
		SDL_RenderFillRect(ife->renderer, rect_reader + i);
		SDL_RenderCopy(ife->renderer,
			(SDL_Texture *)texture_reader[i], NULL, rect_reader + i);
		i++;
	}
	return (0);
}

int	widget_change_elem_status(void *widget, int x, int y)
{
	uint32_t		i;
	uint32_t		widget_elem_count;
	const SDL_Rect	*rect_reader;
	SDL_Color		*color_writer;

	widget_elem_count = *(uint32_t *)widget;
	rect_reader = ((void **)(widget + sizeof(uint32_t)))[0];
	if (widget_elem_count <= 2
		|| !(x > rect_reader[0].x && x < (rect_reader[0].x + rect_reader[0].w)))
		return (0);
	i = 2;
	widget += sizeof(uint32_t);
	color_writer = ((void **)widget)[1];
	while (i != widget_elem_count && !((x >= rect_reader[i].x
				&& x <= (rect_reader[i].x + rect_reader[i].w))
			&& (y >= rect_reader[i].y
				&& y <= (rect_reader[i].y + rect_reader[i].h))))
		i++;
	if (i == widget_elem_count)
		return (0);
	if (color_writer[i].r != 255)
		color_writer[i] = (SDL_Color){255, 0, 0, 16};
	else
		color_writer[i] = (SDL_Color){0, 255, 0, 16};
	return (0);
}

int	widget_scroll(t_ife *ife, void *widget, int px)
{
	uint32_t	i;
	uint32_t	widget_elem_count;
	SDL_Rect	*rect_writer;

	widget_elem_count = *(uint32_t *)widget;
	if (widget_elem_count <= 2)
		return (0);
	rect_writer = ((void **)((void *)widget + sizeof(uint32_t)))[0];
	if (px <= 0 && !(ife->height - TEXT_BOX_PADDING < (
				rect_writer[widget_elem_count - 1].y
				+ rect_writer[widget_elem_count - 1].h)))
		return (0);
	if (px >= 0 && !((rect_writer[1].y + rect_writer[1].h)
			> rect_writer[2].y + TEXT_BOX_PADDING))
		return (0);
	i = 2;
	while (i != widget_elem_count)
	{
		rect_writer[i].y += px;
		i++;
	}
	return (0);
}
