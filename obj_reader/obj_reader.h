#ifndef OBJ_READER_H
#define OBJ_READER_H

#include "libft.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include <stdio.h>

#include <sys/stat.h>

char*	ft_read_file(const char *path);
double	ft_atof(const char *str);

/* numbers section*/
int	is_number(const char *str);
int	is_number_sequence(const char *str, unsigned char sp, unsigned long *bytes);
int	write_number_sequence(const char *str, unsigned char sp,
								unsigned long *bytes, void *dest);

/* definitions section */

/* [char sequence] float_0 ... float_n-1 float_n */
int	is_definition(const char *str, unsigned int offset);
/* [char sequence] float_0 ... float_n-1 float_n */
int	write_definition(const char *str, unsigned int offset, float *dest);
/* [char sequence] float_0/float_0/float_0
... float_n-1/float_n-1/float_n-1 float_n/float_n/float_n */
int	is_definition_complex(const char *str, unsigned int offset);
/* [char sequence] float_0/float_0/float_0
... float_n-1/float_n-1/float_n-1 float_n/float_n/float_n */
void	*write_definition_complex(const char *str,
									unsigned int offset, void *dest);

#endif
