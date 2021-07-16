/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/10 20:48:57 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/15 01:05:47 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

float __attribute__((overloadable)) det(t_matrix2x2 m)
{
	return (m[0] * m[3] - m[1] * m[2]);
}

float __attribute__((overloadable)) det(t_matrix4x4 m)
{
	return (m[0] * det((t_matrix2x2){m[5], m[6], m[9], m[10]})
				- m[1] * det((t_matrix2x2){m[4], m[6], m[8], m[10]})
					+ m[2] * det((t_matrix2x2){m[4], m[5], m[8], m[9]}));
}

t_matrix2x2 __attribute__((overloadable)) transpose(t_matrix2x2 m)
{
	return ((t_matrix2x2){
		m[0], m[2],
		m[1], m[3]
	});
}

t_matrix4x4 __attribute__((overloadable)) transpose(t_matrix4x4 m)
{
	return ((t_matrix4x4){
		m[0], m[4], m[8], m[12],
		m[1], m[5], m[9], m[13],
		m[2], m[6], m[10], m[14],
		m[3], m[7], m[11], m[15]
	});
}

t_matrix2x2 __attribute__((overloadable)) invert(t_matrix2x2 m)
{
	return (1 / det(m) * (t_float4){
		m[3], -m[1],
		-m[2], m[0]
	});
}

t_matrix4x4 __attribute__((overloadable)) invert(t_matrix4x4 m)
{
	/* transposed algebraic adj matrix */
	m = 1 / det(m) * transpose((t_matrix4x4){
		det((t_matrix2x2){m[5], m[6], m[9], m[10]}),
		-det((t_matrix2x2){m[4], m[6], m[8], m[10]}),
		det((t_matrix2x2){m[4], m[5], m[8], m[9]}), 0,
		-det((t_matrix2x2){m[1], m[2], m[9], m[10]}),
		det((t_matrix2x2){m[0], m[2], m[8], m[10]}),
		-det((t_matrix2x2){m[0], m[1], m[8], m[9]}), 0,
		det((t_matrix2x2){m[1], m[2], m[5], m[6]}),
		-det((t_matrix2x2){m[0], m[2], m[4], m[6]}),
		det((t_matrix2x2){m[0], m[1], m[4], m[5]}), 0,
		0, 0, 0, 1
	});
m[15] = 1;
return (m);
}

t_float2 __attribute__((overloadable)) mul(t_matrix2x2 a, t_float2 b)
{
	return ((t_float2){
		a[0] * b[0] + a[1] * b[1],
		a[2] * b[0] + a[3] * b[1]
	});
}

t_float4 __attribute__((overloadable)) mul(t_matrix4x4 a, t_float4 b)
{
	a = transpose(a);
	return transpose(((t_float4 *)&a)[0] * b.x
				+ ((t_float4 *)&a)[1] * b.y + ((t_float4 *)&a)[2] * b.z + ((t_float4 *)&a)[3] * b.w);
}

t_matrix4x4 __attribute__((overloadable)) mul(t_matrix4x4 a, t_matrix4x4 b)
{
	t_matrix4x4 m;

	a = transpose(a);
	b = transpose(b);
	((t_float4 *)&m)[0] = ((t_float4 *)&a)[0] * ((t_float4 *)&b)[0].x
		+ ((t_float4 *)&a)[1] * ((t_float4 *)&b)[0].y
			+ ((t_float4 *)&a)[2] * ((t_float4 *)&b)[0].z
				+ ((t_float4 *)&a)[3] * ((t_float4 *)&b)[0].w;
	((t_float4 *)&m)[1] = ((t_float4 *)&a)[0] * ((t_float4 *)&b)[1].x
		+ ((t_float4 *)&a)[1] * ((t_float4 *)&b)[1].y
			+ ((t_float4 *)&a)[2] * ((t_float4 *)&b)[1].z
				+ ((t_float4 *)&a)[3] * ((t_float4 *)&b)[1].w;
	((t_float4 *)&m)[2] = ((t_float4 *)&a)[0] * ((t_float4 *)&b)[2].x
		+ ((t_float4 *)&a)[1] * ((t_float4 *)&b)[2].y
			+ ((t_float4 *)&a)[2] * ((t_float4 *)&b)[2].z
				+ ((t_float4 *)&a)[3] * ((t_float4 *)&b)[2].w;
	((t_float4 *)&m)[3] = ((t_float4 *)&a)[0] * ((t_float4 *)&b)[3].x
		+ ((t_float4 *)&a)[1] * ((t_float4 *)&b)[3].y
			+ ((t_float4 *)&a)[2] * ((t_float4 *)&b)[3].z
				+ ((t_float4 *)&a)[3] * ((t_float4 *)&b)[3].w;
	return (transpose(m));
}
