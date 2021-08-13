/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniform_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 21:49:20 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/12 21:52:06 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opengl_init.h"

GLint	color_init(t_float4 color, GLint uniform)
{
	glUniform4fv(uniform, 1, (const GLfloat *)&color);
	return (glGetError());
}
