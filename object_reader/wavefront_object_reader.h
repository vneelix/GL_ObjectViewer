/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wavefront_object_reader.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 20:20:17 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/09 21:33:03 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WAVEFRONT_OBJECT_READER_H
# define WAVEFRONT_OBJECT_READER_H

# include "minilib.h"

# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <unistd.h>
# include <sys/stat.h>

# define MAX_NAME_LEN	32
# define MAX_OBJ_COUNT	256

typedef enum type {
	v,
	vn,
	vt,
	f
}	t_type;

/* wavefront definition handler */
int		def_handler(const char *definition,
			unsigned int *out_def, size_t *out_memory_size);

/* wavefront object getter */
int		change_object(const char *definition,
			void **out_object, int rewrite_flag, char **out_err);
void	*create_object_buffer(char **out_err);
void	*wavefront_object_get(const char **wf_file,
			void **container, void **out_object, char **out_err);

/* wavefront defintion writer */
int		def_writer(const char **obj, void *mem, unsigned int *def);

/* numbers section*/
int		is_number_sequence(const char *str,
			unsigned char sp, unsigned long *bytes);
int		write_number_sequence(const char *str, unsigned char sp,
			unsigned long *bytes, void *dest);

/* wavefront object reader */
void	*wavefront_object_reader(const char *path,
			void **out_object, unsigned int **out_def_count, char **out_err);

/* wavefront object definiton */
int		is_definition(const char *str, unsigned int offset);
int		write_definition(const char *str, unsigned int offset, float *dest);
int		is_definition_complex(const char *str, unsigned int offset);
void	*write_definition_complex(const char *str,
			unsigned int offset, void *dest);

#endif
