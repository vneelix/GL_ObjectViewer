/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 23:23:10 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/15 23:27:12 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* memory scheme
** [ptr_vertex, ptr_normal, ptr_texture_coord, ptr_polygon, NULL]
** (float)[vertex_count, v0.x, v0.y, v0.z, (v0.w) ... vn.x, vn.y, vn.z, (vn.w)]
** (float)[normal_count, N0.x, N0.y, N0.z, (N0.w) ... Nn.x, Nn.y, Nn.z, (Nn.w)]
** (float)[normal_count, N0.x, N0.y, N0.z, (N0.w) ... Nn.x, Nn.y, Nn.z, (Nn.w)]
** (float)[texture_coord_count, T0.x, T0.y, (T0.w) ... Tn.x, Tn.y, (Tn.w)]
** [(int)polygon_count, [ptr_poly0 .. ptr_poly_n, NULL] ->
** -> (int)[vertex_count, (vertex0_index,
** vertex0_normal_index, vertex0_texture_coord_index) ...]]
*/

#include "obj_reader.h"

static char	**get_splitted_obj(const char *path)
{
	char	**obj;
	char	*file;

	file = ft_read_file(path);
	if (!file)
		return (NULL);
	obj = ft_strsplit(file, '\n');
	free(file);
	return (obj);
}

static int	calc_memory_size(const char **obj, unsigned long *out_def,
				unsigned long *out_memory_size, const char **out_err_def)
{
	bzero(out_def, sizeof(unsigned long) * 4);
	*out_memory_size = sizeof(void *) * 5 + sizeof(int) + sizeof(void *);
	while (*obj)
	{
		if (def_handler(*obj, out_def, out_memory_size) == -1)
		{
			if (out_err_def)
				*out_err_def = *obj;
			return (-1);
		}
		obj++;
	}
	*out_memory_size += 3 * sizeof(float)
		* (1 + out_def[vt] + sizeof(float) * (out_def[v] + out_def[vn]));
	return (0);
}

static void	*get_memory(const char **obj,
				unsigned long *def_cpy, const char **out_err_def)
{
	void			*mem;
	unsigned long	def[4];
	unsigned long	mem_size;

	if (calc_memory_size(obj, def, &mem_size, out_err_def) == -1)
		return (NULL);
	mem = malloc(mem_size);
	if (!mem)
		return (NULL);
	bzero(mem, mem_size);
	((void **)mem)[0] = mem + 5 * sizeof(void *);
	((void **)mem)[1] = ((void **)mem)[0]
		+ sizeof(float) + 4 * sizeof(float) * def[v];
	((void **)mem)[2] = ((void **)mem)[1]
		+ sizeof(float) + 4 * sizeof(float) * def[vn];
	((void **)mem)[3] = ((void **)mem)[2]
		+ sizeof(float) + 3 * sizeof(float) * def[vt];
	*((float *)((void **)mem)[0]) = def[v];
	*((float *)((void **)mem)[1]) = def[vn];
	*((float *)((void **)mem)[2]) = def[vt];
	*((int *)((void **)mem)[3]) = def[f];
	if (def_cpy)
		memcpy(def_cpy, def, sizeof(unsigned long) * 4);
	return (mem);
}

void	*read_obj(const char *path, void *out_err_def)
{
	void			*mem;
	char			**obj;
	unsigned long	def[4];

	obj = get_splitted_obj(path);
	mem = get_memory((const char **)obj, def, (const char **)out_err_def);
	if (!mem)
	{
		((void **)out_err_def)[0] = strdup(((void **)out_err_def)[0]);
		free(obj);
		return (NULL);
	}
	def_writer((const char **)obj, mem, def);
	free(obj);
	return (mem);
}

/* int main(int argc, char *argv[]) {
	char *err;
	void *obj_raw = read_obj(argv[1], (const char**)(&err));

	if (!obj_raw) {
		printf("%s\n", err);
		free(err);
		return(0);
	}

	int count = 0;

	float *vertices = ((void **)obj_raw)[v];
	count = *vertices;
	vertices++;

	for (int i = 0; i != count; i++) {
		printf("v %f %f %f %f\n", vertices[0], vertices[1], vertices[2], vertices[3]);
		vertices += 4;
	}
	printf("\n");

	float *normals = ((void **)obj_raw)[vn];
	count = *normals;
	normals++;

	for (int i = 0; i != count; i++) {
		printf("vn %f %f %f %f\n", normals[0], normals[1], normals[2], normals[3]);
		normals += 4;
	}
	printf("\n");

	float *textr = ((void **)obj_raw)[vt];
	count = *textr;
	textr++;

	for (int i = 0; i != count; i++) {
		printf("vt %f %f %f\n", textr[0], textr[1], textr[2]);
		textr += 3;
	}
	printf("\n");

	count = *((int*)(((void **)obj_raw)[f]));
	int **f_ptr = ((void **)obj_raw)[f] + sizeof(int);
	for (int i = 0; i != count; i++) {
		printf("f ");
		int c = *(f_ptr[i]);
		int *arr = f_ptr[i] + 1;
		for (int i = 0; i != c; i++) {
			printf("%d/%d/%d ", arr[0], arr[1], arr[2]);
			arr += 3;
		}
		printf("\n");
	}

	free(obj_raw);
	exit(0);
} */
