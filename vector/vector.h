#ifndef VECTOR_H
# define VECTOR_H

# include <math.h>

/* typedefs of vector extensions */

typedef unsigned int	t_uint2 __attribute__((ext_vector_type(2)));
typedef float		t_float2 __attribute__((ext_vector_type(2)));
typedef float		t_float4 __attribute__((ext_vector_type(4)));
typedef float		t_float16 __attribute__((ext_vector_type(16)));
typedef t_float4	t_matrix2x2;
typedef t_float16	t_matrix4x4;
/*  */

/* overloadable vector operations */

float		len(t_float2 a) __attribute__((overloadable));
float		len(t_float4 a) __attribute__((overloadable));
float		len2(t_float2 a) __attribute__((overloadable));
float		len2(t_float4 a) __attribute__((overloadable));
float		dot(t_float2 a, t_float2 b) __attribute__((overloadable));
float		dot(t_float4 a, t_float4 b) __attribute__((overloadable));
t_float2	normalize(t_float2 a) __attribute__((overloadable));
t_float4	normalize(t_float4 a) __attribute__((overloadable));
/*  */

/* vector operations */

t_float4	cross(t_float4 a, t_float4 b);
/*  */

/* overloadable matrix operations */

float		det(t_matrix2x2 m) __attribute__((overloadable));
float		det(t_matrix4x4 m) __attribute__((overloadable));
t_matrix2x2	transpose(t_matrix2x2 m) __attribute__((overloadable));
t_matrix4x4	transpose(t_matrix4x4 m) __attribute__((overloadable));
t_matrix2x2	invert(t_matrix2x2 m) __attribute__((overloadable));
t_matrix4x4	invert(t_matrix4x4 m) __attribute__((overloadable));
t_float2	mul(t_matrix2x2 a, t_float2 b) __attribute__((overloadable));
t_float4	mul(t_matrix4x4 a, t_float4 b) __attribute__((overloadable));
t_matrix4x4	mul(t_matrix4x4 a, t_matrix4x4 b) __attribute__((overloadable));
/*  */
#endif
