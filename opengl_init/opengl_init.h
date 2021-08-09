/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl_init.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 23:07:33 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/10 00:49:48 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_INIT_H
# define OPENGL_INIT_H
# include "minilib.h"
# include "triangulator.h"
# include "wavefront_object_reader.h"

typedef struct s_opengl
{
	GLuint	program;
}	t_opengl;

#endif
