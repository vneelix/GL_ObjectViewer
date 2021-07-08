/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_overloadable_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 21:55:13 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/06 21:55:20 by vneelix          ###   ########.fr       */
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
