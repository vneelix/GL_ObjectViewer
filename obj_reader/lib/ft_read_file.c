#include "obj_reader.h"

char	*ft_read_file(const char *path)
{
	int			fd;
	struct stat	st;
	void		*data;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	fstat(fd, &st);
	if (st.st_size == -1)
	{
		close(fd);
		return (NULL);
	}
	data = malloc(sizeof(char) * (st.st_size + 1));
	if (data == NULL)
	{
		close(fd);
		return (NULL);
	}
	((char *)data)[st.st_size] = 0;
	if (read(fd, data, st.st_size) == -1)
	{
		close(fd);
		free(data);
		return (NULL);
	}
	close(fd);
	return (data);
}
