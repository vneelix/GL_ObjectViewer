/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulator.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 20:42:44 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/14 12:37:39 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRIANGULATOR_H
# define TRIANGULATOR_H

# include "vector.h"
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# define GL_SILENCE_DEPRECATION
# include <OpenGL/gl3.h>
# include "wavefront_object_reader.h"
# define VERTEX_BUFFER_SIZE 256

int			inside_triangle(t_float2 p0, t_float2 p1, t_float2 p2, t_float2 a);

void		int32_stack_shift_up(__int32_t *stack, __uint32_t num);
void		int32_stack_shift_down(__int32_t *stack, __uint32_t num);
void		int64_stack_shift_up(__int64_t *stack, __uint32_t num);
void		int64_stack_shift_down(__int64_t *stack, __uint32_t num);

int			check_collision(void **container, t_float2 *triangle, int *indexes);
int			is_valid_triangle(void **container, int index, int *indexes);
int			get_triangle(void **container, int *out_triangle);
int			triangulate_polygon(t_float2 *polygon,
				size_t vertex_count, int *out_triangle);

const void	*get_elem_from_array(const void *array,
				size_t length, size_t sizeof_elem, long index);
int			get_elem_from_int(const int *array, size_t length, long index);
t_float2	get_elem_from_float2(const t_float2 *array,
				size_t length, long index);
t_float4	get_elem_from_float4(const t_float4 *array,
				size_t length, long index);

size_t		model_triangles_count(
				int **polygon, size_t polygon_count);
int			polygon_handling(t_float4 *vertex,
				size_t vertex_count, int *polygon, int **writer);
int			triangulate_polygon3d(t_float4 *polygon3d,
				size_t polygon3d_vertex_count, int *out_triangle);

int			rewriter(void **data, float *vbo);
int			*wavefront_to_gl_index_converter(t_float4 *vertex,
				size_t vertex_count, int **polygon, size_t polygon_count);
GLuint		*wavefront_to_gl_arrays_converter(
				void **data, void **object, char **err);

int			error_callback(int err_code,
				const char *target, const char *reason, char **err);
long		in_range(long range, long index);

#endif
