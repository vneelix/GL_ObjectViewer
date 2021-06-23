#include "obj_reader.h"

char	**get_splitted_obj(const char *path) {
	char	**obj;
	char	*file;

	file = ft_read_file(path);
	if (!file)
		return (NULL);
	obj = ft_strsplit(file, '\n');
	free(file);
	return (obj);
}

int	def_count(const char **obj, unsigned long *def, unsigned long *mem_size) {
	int count;

	count = 0;
	*mem_size = 6 * sizeof(void*);
	bzero(def, sizeof(unsigned long) * 4);
	while (*obj) {
		if (!strncmp(*obj, "v ", 2) || !strncmp(*obj, "v\t", 2)) {
			def[v]++;
			count = is_definition(*obj, 1);
			if (!(count == 3 || count == 4))
				return (-1);
		}
		else if (!strncmp(*obj, "vn ", 3) || !strncmp(*obj, "vn\t", 3)) {
			def[vn]++;
			count = is_definition(*obj, 2);
			if (!(count == 3 || count == 4))
				return (-1);
		}
		else if (!strncmp(*obj, "vt ", 3) || !strncmp(*obj, "vt\t", 3)) {
			def[vt]++;
			count = is_definition(*obj, 2);
			if (!(count == 2 || count == 3))
				return (-1);
		}
		else if (!strncmp(*obj, "f ", 2) || !strncmp(*obj, "f\t", 2)) {
			def[f]++;
			count = is_definition_complex(*obj, 1);
			if (count < 0)
				return (-1);
			*mem_size += sizeof(void*) + sizeof(int) + count * 3 * sizeof(int);
		}
		obj++;
	}
	*mem_size += 3 * sizeof(float) * (1 + def[vt] + sizeof(float) * (def[v] + def[vn]));
	return (0);
}

int		simple_writer(const char *str, void *dest, unsigned int def) {
	static void *writer[3] = {NULL, NULL, NULL};

	if (!writer[def])
		writer[def] = dest;
	if (def == v)
		write_definition(str, 1, writer[def]);
	else
		write_definition(str, 2, writer[def]);
	writer[def] += sizeof(float) * 4;
	return (0);
}

int 	complex_writer(const char *str, void *dest, unsigned long count) {
	static void *writer_ptr = NULL;
	static void *writer_data = NULL;

	if (!writer_ptr)
		writer_ptr = dest;
	if (!writer_data)
		writer_data = dest + sizeof(void*) * (count + 1);
	*((void **)writer_ptr) = writer_data;
	writer_ptr += sizeof(void*);
	writer_data = write_definition_complex(str, 1, writer_data);
	return 0;
}

int		def_writer(const char **obj, void *mem, unsigned long *def) {
	while (*obj) {
		if (!strncmp(*obj, "v ", 2) || !strncmp(*obj, "v\t", 2))
			simple_writer(*obj, ((void **)mem)[v] + sizeof(float), v);
		else if (!strncmp(*obj, "vn ", 3) || !strncmp(*obj, "vn\t", 3))
			simple_writer(*obj, ((void **)mem)[vn] + sizeof(float), vn);
		else if (!strncmp(*obj, "vt ", 3) || !strncmp(*obj, "vt\t", 3))
			simple_writer(*obj, ((void **)mem)[vt] + sizeof(float), vt);
		else if (!strncmp(*obj, "f ", 2) || !strncmp(*obj, "f\t", 2))
			complex_writer(*obj, ((void **)mem)[f], def[f]);
		obj++;
	}
	return (0);
}

void	*get_memory(const char **obj, unsigned long *def_cpy) {
	void			*mem;
	unsigned long	def[4];
	unsigned long	mem_size;

	def_count(obj, def, &mem_size);
	mem = malloc(mem_size);
	if (!mem)
		return (NULL);
	bzero(mem, mem_size);
	((void **)mem)[0] = mem + 5 * sizeof(void*);
	((void **)mem)[1] = ((void **)mem)[0] + sizeof(float) + 4 * sizeof(float) * def[v];
	((void **)mem)[2] = ((void **)mem)[1] + sizeof(float) + 4 * sizeof(float) * def[vn];
	((void **)mem)[3] = ((void **)mem)[2] + sizeof(float) + 3 * sizeof(float) * def[vt];
	*((float*)((void **)mem)[0]) = def[v];
	*((float*)((void **)mem)[1]) = def[vn];
	*((float*)((void **)mem)[2]) = def[vt];
	if (def_cpy)
		memcpy(def_cpy, def, sizeof(unsigned long) * 4);
	return (mem);
}

void	*read_obj(const char *path) {
	void			*mem;
	char			**obj;
	unsigned long	def[4];

	obj = get_splitted_obj(path);
	mem = get_memory((const char **)obj, def);
	def_writer((const char **)obj, mem, def);

/* 	int count = 0;

	float *vertices = ((void **)mem)[0];
	count = *vertices;
	vertices++;

	for (int i = 0; i != count; i++) {
		printf("v %f %f %f %f\n", vertices[0], vertices[1], vertices[2], vertices[3]);
		vertices += 4;
	}

	float *normals = ((void **)mem)[1];
	count = *normals;
	normals++;

	for (int i = 0; i != count; i++) {
		printf("vn %f %f %f %f\n", normals[0], normals[1], normals[2], normals[3]);
		normals += 4;
	}

	int **f_ptr = ((void **)mem)[3];
	for (; *f_ptr; f_ptr++) {
		printf("f ");
		count = **f_ptr;
		int *arr = *f_ptr + 1;
		for (int i = 0; i != count; i++) {
			printf("%d/%d/%d ", arr[0], arr[1], arr[2]);
			arr += 3;
		}
		printf("\n");
	} */

	free(mem);
	free(obj);

	return (NULL);
}

int main(int argc, char *argv[]) {
	void *obj_raw = read_obj(argv[1]);
	exit(0);
}
