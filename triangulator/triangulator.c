/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 22:11:36 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/06 23:12:13 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "triangulator.h"

#include <stdio.h>

int	triangulate(const t_float2 *vertices, int vertices_count,
				const int *polygon, int polygon_vert_count, int *out_triangles)
{
	return (0);
}

int	main() {
	float vertices[] = {
		-3, 2.87,
		-0.71, 1.79,
		1.9, 3.29,
		3.77, -1.61,
		-2.2, -2.76,
		-3.84, 0
	};

	int	polygon[] = {
		0, 1, 2, 3, 4, 5
	};

	int triangles[(sizeof(vertices) / (2 * sizeof(float)) - 2) * 3] = {0};

	triangulate((t_float2*)(&(vertices[0])), sizeof(vertices) / (2 * sizeof(float)),
				polygon, sizeof(polygon) / sizeof(float), triangles);

	return (0);
}
