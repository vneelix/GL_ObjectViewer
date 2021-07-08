#ifndef VECTOR_H
# define VECTOR_H

# include <math.h>

/* typedefs of vector extensions */

typedef float	t_float2 __attribute__((ext_vector_type(2)));
typedef float	t_float4 __attribute__((ext_vector_type(4)));
/*  */

/* overloadable vector operations */

float		len(t_float2 a) __attribute__((overloadable));
float		len(t_float4 a) __attribute__((overloadable));
float		dot(t_float2 a, t_float2 b) __attribute__((overloadable));
float		dot(t_float4 a, t_float4 b) __attribute__((overloadable));
t_float2	normalize(t_float2 a) __attribute__((overloadable));
t_float4	normalize(t_float4 a) __attribute__((overloadable));
/*  */

/* vector operations */

t_float4	cross(t_float4 a, t_float4 b);
/*  */
#endif
