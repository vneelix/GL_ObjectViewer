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

void*	get_obj(const char *path) {
	char *file = ft_read_file(path);
	if (!file)
		return NULL;

	char **data = ft_strsplit((const char*)file, '\n');
	free(file);
	if (!data)
		return NULL;

	int count_f = 0;

	for (int i = 0; data[i]; i++) {
		if (memcmp(data[i], "vn", 2) == 0)
			type_size[vn]++;
		else if (memcmp(data[i], "vt", 2) == 0)
			type_size[vt]++;
		else if (data[i][0] == 'v')
			type_size[v]++;

		int c;
		if (data[i][0] == 'f') {
			count_f++;
			type_size[f] += sizeof(void*);
			c = is_definition_complex(data[i], 1);
			if (c < 0)
				return NULL;
			type_size[f] += sizeof(int) + c * 3 * sizeof(int);
		}
	}

	float *p0 = vertice = malloc(sizeof(float) * type_size[v] * 4 + sizeof(float));
	float *p1 = normal = malloc(sizeof(float) * type_size[vn] * 4 + sizeof(float));
	*p0++ = type_size[v];
	*p1++ = type_size[vn];

	bzero(p0, sizeof(float) * type_size[v] * 4);
	bzero(p1, sizeof(float) * type_size[vn] * 4);

	type_size[f] += sizeof(void*);
	void *poly = malloc(type_size[f]);
	bzero(poly, type_size[f]);

	void *poly_writer = poly;
	void *poly_data_writer = poly + (count_f + 1) * sizeof(void*);

	for( int i = 0; data[i]; i++) {
		int c = 0;
		if (memcmp(data[i], "vn", 2) == 0) {
			c = is_definition(data[i], 2);
			if (c == 3) {
				write_definition(data[i], 2, p1);
				p1 += 4;
			}
		}
		else if (data[i][0] == 'v') {
			c = is_definition(data[i], 1);
			if (c == 3 || c == 4) {
				write_definition(data[i], 1, p0);
				if (c == 3)
					p0[3] = 1;
				p0 += 4;
			}
		}
		else if (data[i][0] == 'f') {
			*((unsigned long*)poly_writer) = poly_data_writer;
			poly_writer += sizeof(void*);
			poly_data_writer = write_definition_complex(data[i], 1, poly_data_writer);
		}
	}

	for (int i = 0; ((int**)poly)[i]; i++) {
		int *ptr = &((int**)poly)[i][1];
		for (int j = 0; j != ((int**)poly)[i][0]; j++) {
			printf("%d/%d/%d ", ptr[0], ptr[1], ptr[2]);
			ptr += 3;
		}
		printf("\n");
	}

	free(data);

	/* free(poly);
	free(normal);
	free(vertice); */

	void *object_raw = malloc(sizeof(void*) * 4); /* vertices + normals + polygons (f) + NULLPTR = 4 */
	bzero(object_raw, sizeof(void*) * 4);

	*((void**)object_raw) = vertice;
	*((void**)object_raw + sizeof(void*)) = normal;
	*((void**)object_raw + 2 * sizeof(void*)) = poly;

	return object_raw;
}

int main(int argc, char *argv[]) {
	void *object_raw = get_obj(argv[1]);

	float	*vertices = *((void**)object_raw);
	float	*normals = *((void**)object_raw + sizeof(void*));
	int		**poly = *((void**)object_raw + 2 * sizeof(void*));

	exit(0);
}
