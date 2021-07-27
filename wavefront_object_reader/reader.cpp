/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 23:23:10 by vneelix           #+#    #+#             */
/*   Updated: 2021/07/26 17:56:23 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* memory scheme
** [ptr_vertex, ptr_normal, ptr_texture_coord, ptr_polygon, NULL]
** (float)[vertex_count, v0.x, v0.y, v0.z, (v0.w) ... vn.x, vn.y, vn.z, (vn.w)]
** (float)[normal_count, N0.x, N0.y, N0.z, (N0.w) ... Nn.x, Nn.y, Nn.z, (Nn.w)]
** (float)[normal_count, N0.x, N0.y, N0.z, (N0.w) ... Nn.x, Nn.y, Nn.z, (Nn.w)]
** (float)[texture_coord_count, T0.x, T0.y, (T0.w) ... Tn.x, Tn.y, (Tn.w)]
** [(uint)polygon_count, [ptr_poly0 .. ptr_poly_n, NULL] ->
** -> (uint)[vertex_count, (vertex0_index,
** vertex0_normal_index, vertex0_texture_coord_index) ...]]
*/

#include "wavefront_object_reader.h"

void	strtrim(char *str) {
	char *ptr = str;
	if (*str)
		str += strlen(str) - 1;
	while (str >= ptr && (*str == ' ' || *str == '\t' || *str == '\r')) {
		*str = 0;
		str--;
	}
}

char	*ltrim(const char *str, const char *regex)
{
	char		r[8];
	size_t		regex_len;

	bzero(r, 8);
	regex_len = strlen(regex);
	if (regex_len > 8)
		memcpy(r, regex, 8);
	else
		memcpy(r, regex, regex_len);
	while (*str)
	{
		if (*str == r[0] || *str == r[1] || *str == r[2] || *str == r[3]
			|| *str == r[4] || *str == r[5] || *str == r[6] || *str == r[7])
			str++;
		else
			break;
	}
	return (strdup(str));
}

static char	**get_splitted_obj(const char *path)
{
	char	**obj;
	char	*file;

	file = ft_read_file(path);
	if (!file)
		return (NULL);
	obj = ft_strsplit(file, '\n');
	for (char **ptr = obj; *ptr; ptr++) {
		strtrim(*ptr);
	}
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
	void			**mem;
	unsigned long	def[4];
	unsigned long	mem_size;

	if (calc_memory_size(obj, def, &mem_size, out_err_def) == -1)
		return (NULL);
	mem = malloc(mem_size);
	if (!mem)
		return (NULL);
	bzero(mem, mem_size);
	mem[v] = mem + 5;
	mem[vn] = mem[v] + sizeof(float) + def[v] * sizeof(float) * 4;
	mem[vt] = mem[vn] + sizeof(float) + def[vn] * sizeof(float) * 4;
	mem[f] = mem[vt] + sizeof(float) + def[vt] * sizeof(float) * 3;
	((float *)mem[v])[0] = def[v];
	((float *)mem[vn])[0] = def[vn];
	((float *)mem[vt])[0] = def[vt];
	((int *)mem[f])[0] = (int)def[f];
	if (def_cpy)
		memcpy(def_cpy, def, sizeof(unsigned long) * 4);
	return (mem);
}

/**
 * @deprecated
*/
/* void	*read_obj(const char *path, void *out_err_def)
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
} */

int	write_object_name(const char *definition, void *object) {
	size_t	len;

	if (*definition)
		definition++;
	while (*definition == ' '
			|| *definition == '\t')
		definition++;
	len = strlen(definition);
	if (len < MAX_NAME_LEN)
		memcpy(object, definition, len);
	else
		memcpy(object, definition, MAX_NAME_LEN - 1);
	return (0);
}

int	change_object(const char *definition, void **object, int rewrite_flag) {
	int	*def;

	def = (int*)(object[0] + MAX_NAME_LEN);
	if ((def[v] == 0 || def[f] == 0)
		&& (def[vn] != 0 || def[vt] != 0))
		return (-1);
	if (def[v] == 0 && def[f] == 0) {
		if (rewrite_flag)
			write_object_name(definition, object[0]);
		return (0);
	}

	write_object_name(definition, object[1]);
	int *offset = object[0] + MAX_NAME_LEN;
	int temp[4];
	temp[0] = offset[0] + offset[4 + 0];
	temp[1] = offset[1] + offset[4 + 1];
	temp[2] = offset[2] + offset[4 + 2];
	temp[3] = offset[3] + offset[4 + 3];
	memcpy(object[1] + MAX_NAME_LEN + sizeof(int) * 4, temp, sizeof(int) * 4);
	return (1);
}

int	analyze_wavefront_obj(const char **obj, unsigned long *out_def,
							size_t *out_memory_size, void **out_err_def, void **out_object)
{
	int	i;
	int	ret;

	i = 0;

	int *def_ptr = out_object[i] + MAX_NAME_LEN;

	while (*obj != NULL)
	{
		if (def_handler(*obj, out_object[i] + MAX_NAME_LEN, out_memory_size) == -1)
		{
			if (out_err_def)
				*out_err_def = strdup((void *)(*obj));
			return (-1);
		}
		if (!strncmp(*obj, "o ", 2) || !strncmp(*obj, "o\t", 2))
		{
			if (i == MAX_OBJ_COUNT) {
				if (out_err_def)
					*out_err_def = strdup("The limit of objects (o) has been reached");
				return (-1);
			}
			(*((__uint128_t *)out_def)) += *((__uint128_t *)(out_object[i] + MAX_NAME_LEN));
			ret = change_object(*obj, out_object + i, i == 0);
			if (ret == -1) {
				if (out_err_def)
					*out_err_def = strdup("The object must have vertices and faces");
				return (-1);
			}
			else if (ret == 1) {
				i++;
				def_ptr = out_object[i] + MAX_NAME_LEN;
			}
		}
		obj++;
	}
	(*((__uint128_t *)out_def)) += *((__uint128_t *)(out_object[i] + MAX_NAME_LEN));
	return ((int)i + 1);
}

void	*read_obj(const char *path, void *out_err_def, void *out_object)
{
	void			**mem;
	char			**obj;
	int				def[4];
	size_t			memory_size;

	obj = get_splitted_obj(path);

	/* markup memory for objects */
	void **object = calloc(sizeof(int) + (sizeof(void *) + MAX_NAME_LEN + 8 * sizeof(int)) * MAX_OBJ_COUNT + sizeof(void *), 1);
	void **temp = (void *)object + sizeof(int);
	for (size_t i = 0; i != MAX_OBJ_COUNT; i++) {
		temp[i] = (void *)(temp + MAX_OBJ_COUNT + 1) + (MAX_NAME_LEN + 8 * sizeof(int)) * i;
	}
	memcpy(temp[0], "default", strlen("default"));

	memory_size = 0;
	bzero(def, sizeof(int) * 4);
	int objects_count = analyze_wavefront_obj((const char **)obj, def, &memory_size, NULL, temp);
	if (objects_count == -1 || objects_count == 0) {
		free(obj);
		free(object);
		return (NULL);
	}
	*(int *)object = objects_count;

	mem = calloc(
		sizeof(void *) * 5
			+ (sizeof(float) + sizeof(float) * 4 * def[v])
				+ (sizeof(float) + sizeof(float) * 4 * def[vn])
					+ (sizeof(float) + sizeof(float) * 3 * def[vt])
						+ (sizeof(int) + memory_size + sizeof(void *))
	, 1);
	mem[v] = mem + 5;
	mem[vn] = mem[v] + sizeof(float) + def[v] * sizeof(float) * 4;
	mem[vt] = mem[vn] + sizeof(float) + def[vn] * sizeof(float) * 4;
	mem[f] = mem[vt] + sizeof(float) + def[vt] * sizeof(float) * 3;
	((float *)mem[v])[0] = def[v];
	((float *)mem[vn])[0] = def[vn];
	((float *)mem[vt])[0] = def[vt];
	((int *)mem[f])[0] = (int)def[f];
	def_writer(obj, mem, def);

	free(obj);
	if (out_object)
		*(void **)out_object = object;
	return (mem);
}

int main(int argc, char *argv[]) {
	char *err;

	void	**obj;
	void	*wavefront_obj_raw = read_obj(argv[1], (const char**)(&err), &obj);

	if (!wavefront_obj_raw) {
		printf("%s\n", err);
		free(err);
		return(0);
	}

	float *vertices = ((void **)wavefront_obj_raw)[v];
	int v_count = *vertices;
	vertices++;

	int **f_ptr = ((void **)wavefront_obj_raw)[f] + sizeof(int);
	int f_count = *((int*)(((void **)wavefront_obj_raw)[f]));

	int count = *(int *)obj;
	void **object = (void *)obj + sizeof(int);

	for (int i = 1; i <= 1; i++) {
		printf("%s\n", object[i]);
		int *def_ptr = object[i] + MAX_NAME_LEN;

		printf("vertices section\n");
		for (int j = 0; j != def_ptr[v]; j++) {
			float *vertice = &vertices[(def_ptr[v + 4] + j) * 4];
			printf("v %f %f %f %f\n", vertice[0], vertice[1], vertice[2], vertice[3]);
		}

		printf("f section\n");
		for (int j = 0; j != def_ptr[f]; j++) {
			printf("f ");
			int c = *(f_ptr[def_ptr[f + 4] + j]);
			int *arr = f_ptr[def_ptr[f + 4] + j] + 1;
			for (int i = 0; i != c; i++) {
				printf("%d/%d/%d ", arr[0], arr[1], arr[2]);
				arr += 3;
			}
			printf("\n");
		}
		printf("-------------\n");
	}

	free(obj);
	free(wavefront_obj_raw);
	exit(0);
}
