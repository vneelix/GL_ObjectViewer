/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 14:40:07 by vneelix           #+#    #+#             */
/*   Updated: 2021/06/21 23:50:30 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	calc_memory_size(const char *str, const char sep,
								uint32_t *memory_size, uint32_t *offset)
{
	const char	*begin;
	const char	*end;

	begin = str;
	while (*begin && *begin == sep)
		begin++;
	while (*begin)
	{
		end = begin;
		while (*end && *end != sep)
			end++;
		if (begin == end)
			break ;
		*offset += sizeof(void *);
		*memory_size += sizeof(void *) + end - begin + 1;
		while (*end && *end == sep)
			end++;
		begin = end;
	}
}

static void	fill_memory(const char *str, const char sep,
									void *memory_area, uint32_t offset)
{
	const char	*begin;
	const char	*end;
	char		**ptr_area;
	char		*character_area;

	begin = str;
	ptr_area = (char **)memory_area;
	character_area = (char *)(memory_area + offset);
	while (*begin && *begin == sep)
		begin++;
	while (*begin)
	{
		end = begin;
		while (*end && *end != sep)
			end++;
		if (begin == end)
			break ;
		*ptr_area++ = ft_memcpy(character_area, begin, end - begin);
		character_area += end - begin + 1;
		while (*end && *end == sep)
			end++;
		begin = end;
	}
}

/**
 * Function for dividing a string into substrings by separator
 * @param str target string
 * @param sep separator for string
 * @return Array of strings.
 * Attention returned strings are contained in a single section of memory,
 * so to clear it is enough to free only the returned pointer
 */
char	**ft_strsplit(const char *str, const char sep)
{
	uint32_t	offset;
	uint32_t	memory_size;
	void		*memory_area;

	offset = sizeof(void *);
	memory_size = sizeof(void *);
	calc_memory_size(str, sep, &memory_size, &offset);
	memory_area = ft_memalloc(memory_size);
	if (!memory_area)
		return (NULL);
	fill_memory(str, sep, memory_area, offset);
	return (memory_area);
}
