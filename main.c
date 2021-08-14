/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 00:35:09 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 17:01:31 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_project.h"

int	renderer_print_objects(t_gl_renderer *rndr)
{
	uint32_t	i;
	char		*p;

	i = 0;
	p = ft_itoa(rndr->obj_count);
	write(1, "Available objects:", strlen("Available objects:"));
	write(1, p, strlen(p));
	write(1, "\n", 1);
	free(p);
	while (i != rndr->obj_count)
	{
		write(1, ((void **)((void *)rndr->object + sizeof(uint32_t)))[i],
			strlen(((void **)((void *)rndr->object + sizeof(uint32_t)))[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

int	renderer_init(t_gl_renderer *rndr, const char *file, char **err)
{
	if (ife_init(&rndr->ife, rndr->canvas.x, rndr->canvas.y, "3DViewer"))
		return (-1);
	if (gl_renderer_init(rndr, file, err))
		return (ife_cleanup_callback(&rndr->ife, -1));
	renderer_print_objects(rndr);
	free(rndr->object);
	rndr->object = NULL;
	if (rndr->object_index != -1)
	{
		rndr->object_index = in_range(rndr->obj_count, rndr->object_index);
		rndr->rendered_obj = 1;
	}
	else
	{
		rndr->object_index = 0;
		rndr->rendered_obj = rndr->obj_count;
	}
	SDL_GL_GetDrawableSize(rndr->ife.window, &rndr->w, &rndr->h);
	glEnable(GL_DEPTH_TEST);
	return (0);
}

int	argv_handling(int argc, char **argv, t_gl_renderer *rndr, char **err)
{
	uint32_t	i;

	if (argc == 1)
		return (error_callback(-1, "usage",
				"wavefront_object_file [-fn -ft {-o number}]", err));
	rndr->object_index = -1;
	i = 1;
	while (i != (uint32_t)argc)
	{
		if (!strcmp("-fn", argv[i]))
			rndr->flags |= 1;
		else if (!strcmp("-ft", argv[i]))
			rndr->flags |= 1 << 1;
		else if (!strcmp("-o", argv[i]) && i != (uint32_t)argc - 1)
			rndr->object_index = ft_atol(argv[i + 1]);
		i++;
	}
	return (0);
}

int	loop(t_gl_renderer *rndr)
{
	SDL_Event	evt;
	const Uint8	*state;
	uint8_t		flags;

	flags = 0;
	glUseProgram(rndr->gl.program);
	while (1)
	{
		while (SDL_PollEvent(&evt))
		{
			state = SDL_GetKeyboardState(NULL);
			if (state == NULL
				|| evt.type == SDL_QUIT
				|| state[SDL_SCANCODE_ESCAPE])
				return (0);
			event_handler(rndr, state, &evt, &flags);
			renderer(rndr, &flags);
		}
	}
	glUseProgram(0);
	return (0);
}

int	main(int argc, char **argv)
{
	char			*err;
	t_gl_renderer	rndr;

	bzero(&rndr, sizeof(rndr));
	if (argv_handling(argc, argv, &rndr, &err))
	{
		write(1, err, strlen(err));
		write(1, "\n", 1);
		free(err);
		return (-1);
	}
	rndr.fov = 60 * M_PI / 180.;
	rndr.canvas = (t_uint2){W, H};
	if (renderer_init(&rndr, argv[1], &err))
	{
		write(1, err, strlen(err));
		write(1, "\n", 1);
		free(err);
		return (-1);
	}
	loop(&rndr);
	ife_cleanup_callback(&rndr.ife, 0);
	return (gl_renderer_release(&rndr, 0));
}
