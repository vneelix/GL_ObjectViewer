#include "wavefront_object_reader.h"

int main(int argc, char *argv[]) {

	if (argc != 2)
		return (-1);
		char *err;

		void	**obj;
		unsigned int def_count[4] = {0};
		void	*wavefront_obj_raw = wavefront_object_reader((const char *)argv[1],
					(void **)&obj, (unsigned int **)def_count, (char **)&err);

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

	for (int i = 0; i != count; i++) {
		printf("%s\n", object[i]);
		int *def_ptr = object[i] + MAX_NAME_LEN;

		for (int j = 0; j != def_ptr[v]; j++) {
			float *vertice = &vertices[(def_ptr[v + 4] + j) * 4];
			printf("v %f %f %f %f\n", vertice[0], vertice[1], vertice[2], vertice[3]);
		}

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
		printf("\n");
	}

	free(obj);
	free(wavefront_obj_raw);
	exit(0);
}
