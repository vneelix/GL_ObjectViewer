#include "obj_reader.h"

typedef enum type {
	v,
	vn,
	vt,
	f
} t_type;

float *vertice;
float *normal;
float *surface;
int type_size[] = {0, 0, 0, 0};

int	check_number(const char *str) {
	int point = 0;
	const char *p = str;

	if (*str == '.' || *str == '+' || *str == '-')
		if (*str++ == '.')
			point = 1;
	if (!(*str >= '0' && *str <= '9'))
			return 0;
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9')) {
			if (*str != '.' || point)
				break;
			else
				point = 1;
		}
		str++;
	}
	return str - p;
}

int check_number_sequence(const char *str, unsigned char sp, unsigned long *bytes) {
	int num = 0;

	if (bytes)
		*bytes = str;
	while (*str) {
		int offset = check_number(str);
		if (!offset)
			return -1;
		num++;
		str += offset;
		if (*str != sp)
			break;
		str++;
		while (*str == sp) {
			str++;
			num++;
		}
	}
	if (bytes)
		*bytes = str - *bytes;
	return num;
}

int	check_definition(const char *str, unsigned int offset) {
	/* [char sequence] float_0 ... float_n-1 float_n */
	int coord = 0;

	str += offset;
	if (*str != ' ' && *str != '\t')
		return 0;
	while (*str) {
		while (*str == ' ' || *str == '\t')
			str++;
		int offset = check_number(str);
		if (!offset)
			return -1;
		coord++;
		str += offset;
	}
	return coord;
}

int	write_definition(const char *str, unsigned int offset, float *dest) {
	/* [char sequence] float_0 ... float_n-1 float_n */
	str += offset;
	while (*str) {
		while (*str == ' ' || *str == '\t')
			str++;
		*dest++ = ft_atof(str);
		while (*str && *str != ' ' && *str != '\t')
			str++;
	}
	return 0;
}

int	check_definition_complex(const char *str, unsigned int offset) {
	/* [char sequence] float_0/float_0/float_0 ... float_n-1/float_n-1/float_n-1 float_n/float_n/float_n */
	int coord = 0;

	str += offset;
	if (*str != ' ' && *str != '\t')
		return 0;
	while (*str) {
		while (*str == ' ' || *str == '\t')
			str++;
		unsigned long bytes;
		int num = check_number_sequence(str, '/', &bytes);
		if (!(num ==1 || num == 3))
			return -1;
		coord++;
		str += bytes;
	}
	return coord;
}

void*	get_obj(const char *path) {
	char *file = ft_read_file(path);
	if (!file)
		return NULL;

	char **data = ft_strsplit((const char*)file, '\n');
	free(file);
	if (!data)
		return NULL;

	for (int i = 0; data[i]; i++) {
		if (memcmp(data[i], "vn", 2) == 0)
			type_size[vn]++;
		else if (memcmp(data[i], "vt", 2) == 0)
			type_size[vt]++;
		else if (data[i][0] == 'v')
			type_size[v]++;

		else if (data[i][0] == 'f')
			type_size[f]++;
	}

	float *p0 = vertice = malloc(sizeof(float) * type_size[v] * 4);
	float *p1 = normal = malloc(sizeof(float) * type_size[vn] * 4);

	bzero(p0, sizeof(float) * type_size[v] * 4);
	bzero(p1, sizeof(float) * type_size[vn] * 4);

	for( int i = 0; data[i]; i++) {
		int c = 0;
		if (memcmp(data[i], "vn", 2) == 0) {
			c = check_definition(data[i], 2);
			if (c == 3) {
				write_definition(data[i], 2, p1);
				p1 += 4;
			}
		}
		else if (data[i][0] == 'v') {
			c = check_definition(data[i], 1);
			if (c == 3 || c == 4) {
				write_definition(data[i], 1, p0);
				if (c == 3)
					p0[3] = 1;
				p0 += 4;
			}
		}
		else if (data[i][0] == 'f') {
			c = check_definition_complex(data[i], 1);
			if (c < 0)
				return NULL;
		}
	}

	free(data);
	free(vertice);
	return NULL;
}

int main(int argc, char *argv[]) {
	get_obj(argv[1]);
	exit(0);
}
