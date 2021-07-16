/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_overloadable_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 21:55:13 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/10 22:21:03 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_float2 __attribute__((overloadable)) normalize(t_float2 a)
{
	return (a / len(a));
}

t_float4 __attribute__((overloadable)) normalize(t_float4 a)
{
	return (a / len(a));
}

float __attribute__((overloadable)) len2(t_float2 a)
{
	return (a.x * a.x + a.y * a.y);
}

float __attribute__((overloadable)) len2(t_float4 a)
{
	return (a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}
