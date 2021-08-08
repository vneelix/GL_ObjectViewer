#ifndef OBJ_READER_H
#define OBJ_READER_H

#include "minilib.h"

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>

# define MAX_NAME_LEN	32
# define MAX_OBJ_COUNT	256

typedef enum type {
	v,
	vn,
	vt,
	f
} t_type;

/* wavefront definition handler */
int	def_handler(const char *definition,
	unsigned int *out_def, size_t *out_memory_size);

/* wavefront object getter */
int	change_object(const char *definition,
		void **out_object, int rewrite_flag);
void	*create_object_buffer(char **out_err);
void	*wavefront_object_get(const char **wf_file,
			void **container, void **out_object, char **out_err);

/* wavefront defintion writer */
int	def_writer(const char **obj, void *mem, unsigned int *def);

/* numbers section*/
int	is_number_sequence(const char *str, unsigned char sp, unsigned long *bytes);
int	write_number_sequence(const char *str, unsigned char sp,
								unsigned long *bytes, void *dest);

/* wavefront object reader */
int	error_callback(int err_code, const char *err_message, char **out_err);
void	*wavefront_object_reader(const char *path,
			void **out_object, unsigned int **out_def_count, char **out_err);

/* wavefront object definiton */
int	is_definition(const char *str, unsigned int offset);
int	write_definition(const char *str, unsigned int offset, float *dest);
int	is_definition_complex(const char *str, unsigned int offset);
void	*write_definition_complex(const char *str,
									unsigned int offset, void *dest);

#endif
