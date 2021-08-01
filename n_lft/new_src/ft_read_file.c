#include "libft.h"

static char	*read_file(int fd)
{
	struct stat	st;
	ssize_t		ret;
	char		*data;

	fstat(fd, &st);
	if (st.st_size == -1)
		return (NULL);
	data = calloc(st.st_size + 1, sizeof(char));
	if (data == NULL)
		return (NULL);
	ret = read(fd, data, st.st_size);
	if (ret != st.st_size)
	{
		free(data);
		return (NULL);
	}
	return (data);
}

char	*ft_read_file_fstat(const char *path)
{
	int		fd;
	char	*data;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	data = read_file(fd);
	close(fd);
	return (data);
}
