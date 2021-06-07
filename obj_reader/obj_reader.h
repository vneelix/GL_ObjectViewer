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

#endif
