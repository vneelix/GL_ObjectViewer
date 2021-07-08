/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_overloadable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 13:19:25 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/06 21:54:42 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

float __attribute__((overloadable)) len(t_float2 a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

float __attribute__((overloadable)) len(t_float4 a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

float __attribute__((overloadable)) dot(t_float2 a, t_float2 b)
{
	return (a.x * b.x + a.y * b.y);
}

float __attribute__((overloadable)) dot(t_float4 a, t_float4 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}
