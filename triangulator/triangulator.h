#ifndef TRIANGULATOR_H
# define TRIANGULATOR_H

# include "vector.h"

# include <stdlib.h>
# include <string.h>
# include <strings.h>

# include <stdio.h>

# define VERTEX_BUFFER_SIZE 256

/* integer stack operations */

void		int32_stack_shift_up(__int32_t *stack, __uint32_t num);
void		int32_stack_shift_down(__int32_t *stack, __uint32_t num);
void		int64_stack_shift_up(__int64_t *stack, __uint32_t num);
void		int64_stack_shift_down(__int64_t *stack, __uint32_t num);
/*  */

/* utils */

long		in_range(long range, long index);
const void	*get_elem_from_array(const void *array, size_t length,
				size_t sizeof_elem, long index);
int			get_elem_from_int(const int *array, size_t length, long index);
t_float2	get_elem_from_float2(const t_float2 *array,
				size_t length, long index);
t_float4	get_elem_from_float4(const t_float4 *array,
									size_t length, long index);
/*  */

/* inside triangle */

int			inside_triangle(t_float2 p0, t_float2 p1, t_float2 p2, t_float2 a);
/*  */

float		*wavefront_to_gl_vbo_converter(t_float4 *vertex,
				t_float4 *normal, float *texture_coord, void *polygon);
#endif
