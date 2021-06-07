#include "obj_reader.h"

char*	ft_read_file(const char *path) {
	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		return NULL;
	}

	struct stat st;
	fstat(fd, &st);
	long size = st.st_size;
	if (size == -1) {
		close(fd);
		return NULL;
	}

	void *data = malloc(sizeof(char) * (size + 1));
	if (!data) {
		close(fd);
		return NULL;
	}
	ft_bzero(data, size + 1);
	if (read(fd, data, size) == -1) {
		close(fd);
		free(data);
		return NULL;
	}
	close(fd);
	return data;
}
