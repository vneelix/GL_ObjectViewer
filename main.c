#include "opengl_project.h"
#include "triangulator.h"
#include "wavefront_object_reader.h"

GLFWwindow *window = null;

size_t	v_count;
GLfloat *vertex_buffer;

size_t	i_count;
GLuint *index_buffer;

size_t	n_count;
GLfloat	*normal_buffer;

GLuint VBO[3];

GLuint VAO_id = 0;

GLfloat	*vbo_model;

int	VBO_init() {
	/* Get buffer object id's */
	glGenBuffers(sizeof(VBO) / sizeof(GLuint), VBO);
	/* bind vertex buffer object */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	/* Write data into buffer object */
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v_count * 4, vertex_buffer, GL_STATIC_DRAW);
	/* Unbind vertex buffer */
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * i_count * 3, index_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, n_count * 2 * sizeof(t_float4), vbo_model, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

int VAO_init() {
	/* Gen vertex array's list */
	glGenVertexArrays(1, &VAO_id);

	glBindVertexArray(VAO_id);

	

	/* Init coord buffer in VAO */
	glEnableVertexArrayAttrib(VAO_id, 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, null);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexArrayAttrib(VAO_id, 1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, null);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Init color buffer in VAO */
	// glEnableVertexArrayAttrib(VAO_id, 1);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	// glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, null);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Init normal buffer in VAO */
	// glEnableVertexArrayAttrib(VAO_id, 2);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	// glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, null);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return 0;
}

int glfw_init() {
	if (glfwInit() != GLFW_TRUE) {
		char *message = "An error occurred during initialization glfw\n";
		write(2, message, strlen(message));
		return -1;
	}
	return 0;
}

int glfw_init_window(const int weight, const int height, const char *title) {
	window = glfwCreateWindow(
		weight, height, title, NULL, NULL
	);
	if (window == null) {
		char *message = "An error occurred during initialization window\n";
		write(2, message, strlen(message));
		return -1;
	}
	return 0;
}

int glad_init() {
	if (gladLoadGL(glfwGetProcAddress) == 0) {
		char *message = "An error occurred during load opengl functions\n";
		write(2, message, strlen(message));
		return -1;
	}
	return 0;
}

GLuint create_program();

void projection_matrix(float l, float r, float b, float t, float n, float f, GLuint uni) {
	float matrix[] = {
		2 * n / (r - l), 0, 0, 0,
		0, 2 * n / (t - b), 0, 0,
		(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1,
		0, 0, -2 * f * n / (f - n), 0
	};
	glUniformMatrix4fv(uni, 1, GL_FALSE, matrix);
}

void perspective_matrix(double fov, double aspect, double n, double f, GLuint uniform) {
	double cot_half_fov = 1. / tan(fov / 2.);
	float matrix[] = {
		cot_half_fov / aspect, 0, 0, 0,
		0, cot_half_fov, 0, 0,
		0, 0, (n + f) / (n - f), 2 * n * f / (n - f),
		0, 0, -1, 0
	};
	glUniformMatrix4fv(uniform, 1, GL_TRUE, matrix);
}

void rotation_matrix(double angle, double x, double y, double z, GLuint uni) {
	double s = sin(angle), c = cos(angle);
	float matrix[] = {
		x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0,
		y * x * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s, 0,
		x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c, 0,
		0, 0, 0, 1
	};
	glUniformMatrix4fv(uni, 1, GL_TRUE, matrix);
}

void tranlsation_matrix(double x, double y, double z, GLuint uni) {
	float matrix[] = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	};
	glUniformMatrix4fv(uni, 1, GL_TRUE, matrix);
}

t_float4 calc_triangle_normal(t_float4 p1, t_float4 p2, t_float4 p3) {
	t_float4 U, V;

	U = p2 - p1;
	V = p3 - p2;
	t_float4 n = normalize(cross(U, V));
	return n;
}

int func(const char *path, int n) {
	void	**data;

	char *err;

	void **object;

	data = (void **)wavefront_object_reader(path, &object, NULL, &err);

	if (!data) {
		printf("%s\n", err);
		exit(0);
	}

	size_t		vertex_count = *((float *)data[v]);
	float		*vertex = (float *)(data[v] + sizeof(float));

	/*  */
	v_count = vertex_count;
	vertex_buffer = vertex;
	/*  */

	int			polygon_count = *((int*)data[f]);
	int			**polygon = data[f] + sizeof(int);

	int		object_count = *(int*)object;
	void	*mesh = ((void **)((void*)object + sizeof(int)))[n];

	/* for (int i = 0; i != object_count; i++) {
		printf("%d %s\n", i, ((void**)((void*)object + sizeof(int)))[i]);
	} */

	int		mesh_polygon_count = ((int *)(mesh + MAX_NAME_LEN))[f];
	int		**mesh_polygon = polygon + ((int *)(mesh + MAX_NAME_LEN))[f + 4];

	// int *triangles = triangulate_model(vertex, vertex_count, mesh_polygon, mesh_polygon_count);
	int *triangles = triangulate_model(vertex, vertex_count, polygon, polygon_count);

	/*  */
	i_count = *triangles;
	index_buffer = triangles + 1;

	n_count = i_count * 3;
	normal_buffer = calloc(n_count, sizeof(float) * 4);


	t_float4 *v_buffer = calloc(vertex_count, sizeof(t_float4));
	memcpy(v_buffer, vertex_buffer, vertex_count * sizeof(t_float4));

	vbo_model = calloc(n_count * 2, sizeof(t_float4));
	t_float4 *ptr = vbo_model;

	for (int i = 0; i != n_count; i += 3) {
		t_float4 p1, p2, p3;
		p1 = get_elem_from_float4(v_buffer, vertex_count, index_buffer[i]);
		p2 = get_elem_from_float4(v_buffer, vertex_count, index_buffer[i + 1]);
		p3 = get_elem_from_float4(v_buffer, vertex_count, index_buffer[i + 2]);

		t_float4 n = calc_triangle_normal(p1, p2, p3);

		ptr[0] = p1;
		ptr[1] = p2;
		ptr[2] = p3;
		(ptr + n_count)[0] = n;
		(ptr + n_count)[1] = n;
		(ptr + n_count)[2] = n;
		ptr += 3;
	}
	/*  */

	return 0;
}

int	main(int argc, char *argv[]) {

	/* Init glfw library */
	if (glfw_init()) {
		return -1;
	}
	/* Init glfw window */
	if (glfw_init_window(1280, 720, "GLViewer")) {
		glfwTerminate();
		return -1;
	}
	/* Make window's context current */
	glfwMakeContextCurrent(window);

	if (glad_init() != 0) {
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	func(argv[1], (int)ft_atof(argv[2]));
	VBO_init();
	VAO_init();

	/*  */
	GLuint program = create_program();
	if (!program) {
		exit(0);
	}
	glUseProgram(program);

	GLuint p_uni = glGetUniformLocation(program, "ProjectionMatrix");
	GLuint r_uni = glGetUniformLocation(program, "RotationMatrix");
	GLuint t_uni = glGetUniformLocation(program, "TranslationMatrix");

	// projection_matrix(-1, 1, -1, 1, 2, 8, p_uni);
	perspective_matrix(60 * pi / 180., 16. / 9., 2, 256, p_uni);
	rotation_matrix(0, 0, 0, 0, r_uni);
	tranlsation_matrix(0, 0, 0, t_uni);
	/*  */

	double angle = 0;

	double x = 0, y = 0, z = -3;


	while(!glfwWindowShouldClose(window)) {
		// double b = glfwGetTime();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			break;
		}

		/* Rotation block */
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			angle -= M_PI / 180.0;
			rotation_matrix(angle, 1, 0, 0, r_uni);
		} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			angle += M_PI / 180.0;
			rotation_matrix(angle, 1, 0, 0, r_uni);
		} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			angle -= M_PI / 180.0;
			rotation_matrix(angle, 0, 1, 0, r_uni);
		} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			angle += M_PI / 180.0;
			rotation_matrix(angle, 0, 1, 0, r_uni);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			z -= 0.32;
			tranlsation_matrix(0, y, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			z += 0.32;
			tranlsation_matrix(0, y, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			y += 0.32;
			tranlsation_matrix(0, y, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			y -= 0.32;
			tranlsation_matrix(0, y, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			angle -= M_PI / 180.0;
			rotation_matrix(angle, 0, 1, 0, r_uni);
		} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			angle += M_PI / 180.0;
			rotation_matrix(angle, 0, 1, 0, r_uni);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* glBindVertexArray(VAO_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
		glDrawElements(GL_TRIANGLES, i_count * 3, GL_UNSIGNED_INT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0); */

		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)(sizeof(t_float4) * n_count));
		glDrawArrays(GL_TRIANGLES, 0, n_count);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		// double e = glfwGetTime();
		// printf("FPS: %d, framerate: %f\n", (unsigned int)(1000 / (e - b)), e - b);
	}

	glfwTerminate();
	return 0;
}
