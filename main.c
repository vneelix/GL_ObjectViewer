#include "opengl_project.h"
#include "triangulator.h"
#include "wavefront_object_reader.h"
#include <stdio.h>

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

GLFWwindow *window = NULL;

size_t	v_count;
GLfloat *vertex_buffer;

size_t	i_count;
GLuint *index_buffer;

size_t	n_count;
GLfloat	*normal_buffer;

GLuint VBO[3];

GLuint VAO_id = 0;

GLfloat	*vbo_model;

GLuint	vao_id;
GLuint	vbo_id;

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

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * i_count * 3, index_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, n_count * 2 * sizeof(t_float4) + sizeof(t_float2) * n_count, vbo_model, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

int VAO_init() {
	/* Gen vertex array's list */
	GLuint vao[1] = {0};
	glGenVertexArrays(1, vao);

	glBindVertexArray(VAO_id);

	/* Init coord buffer in VAO */
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Init color buffer in VAO */
	// glEnableVertexArrayAttrib(VAO_id, 1);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	// glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Init normal buffer in VAO */
	// glEnableVertexArrayAttrib(VAO_id, 2);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	// glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
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
	if (window == NULL) {
		char *message = "An error occurred during initialization window\n";
		write(2, message, strlen(message));
		return -1;
	}
	return 0;
}

GLuint create_program();

/*int func(const char *path, int n, GLuint program,
			uint32_t w, uint32_t h, GLuint r_uni, GLuint p_uni, GLuint t_uni) {
	void	**data;

	char *err;

	void **object;
	uint32_t	def_count_total[4];

	data = (void **)wavefront_object_reader(path, &object, def_count_total, &err);

	if (!data) {
		printf("%s\n", err);
		exit(0);
	}


	{
		uint32_t	vertex_count = *(float *)data[v];
		float		*vertex = data[v] + sizeof(float);

		t_float4	xMin = {0}, xMax = {0};
		t_float4	yMin = {0}, yMax = {0};
		t_float4	zMin = {0}, zMax = {0};
		for (uint32_t i = 0; i != vertex_count; i++) {
			if (vertex[i * 4] > xMax.x)
				memcpy(&xMax, vertex + i * 4, sizeof(t_float4));
			if (vertex[i * 4 + 1] > yMax.y)
				memcpy(&yMax, vertex + i * 4, sizeof(t_float4));
			if (vertex[i * 4 + 2] > zMax.z)
				memcpy(&zMax, vertex + i * 4, sizeof(t_float4));

			if (vertex[i * 4] < xMin.x)
				memcpy(&xMin, vertex + i * 4, sizeof(t_float4));
			if (vertex[i * 4 + 1] < yMin.y)
				memcpy(&yMin, vertex + i * 4, sizeof(t_float4));
			if (vertex[i * 4 + 2] < zMin.z)
				memcpy(&zMin, vertex + i * 4, sizeof(t_float4));
		}

		float xSegment = fabs(xMax.x - xMin.x);
		float ySegment = fabs(yMax.y - yMin.y);
		float zSegment = fabs(zMax.z - zMin.z);
		GLint e = perspective_projection_matrix(60 * pi / 180., (double)w / (double)h, (t_float2){0.128, 512}, p_uni);
		e = rotation_matrix(0, (t_float4){0, 0, 0, 0}, r_uni);
		e = translation_matrix(-(xSegment / 2 + xMin.x), -(ySegment / 2 + yMin.y), -4 - zSegment, t_uni);
	}

	if (0) {
		*(uint32_t *)object = 1;
		void **obj_ptr = (void *)object + sizeof(uint32_t);
		memset(obj_ptr[0] + MAX_NAME_LEN, 0, sizeof(uint32_t) * 8);
		memcpy(obj_ptr[0] + MAX_NAME_LEN, def_count_total, sizeof(uint32_t) * 4);
	}
	GLuint *arr = wavefront_to_gl_arrays_converter(data, object, &err);
	if (arr == NULL) {
		printf("%s\n", err);
		free(err);
		exit(0);
	}
	n_count = arr[1 + n];
	vao_id = arr[1 + arr[0] + n];

	return (0);
}*/

int func(const char *path, int n, GLuint program,
			uint32_t w, uint32_t h, GLuint r_uni, GLuint p_uni, GLuint t_uni, GLuint *arr) {
	/*void	**data;

	char *err;

	void **object;
	uint32_t	def_count_total[4];

	data = (void **)wavefront_object_reader(path, &object, def_count_total, &err);

	if (!data) {
		printf("%s\n", err);
		exit(0);
	}


	{
		uint32_t	vertex_count = *(float *)data[v];
		float		*vertex = data[v] + sizeof(float);

		t_float4	xMin = {0}, xMax = {0};
		t_float4	yMin = {0}, yMax = {0};
		t_float4	zMin = {0}, zMax = {0};
		for (uint32_t i = 0; i != vertex_count; i++) {
			if (vertex[i * 4] > xMax.x)
				memcpy(&xMax, vertex + i * 4, sizeof(t_float4));
			if (vertex[i * 4 + 1] > yMax.y)
				memcpy(&yMax, vertex + i * 4, sizeof(t_float4));
			if (vertex[i * 4 + 2] > zMax.z)
				memcpy(&zMax, vertex + i * 4, sizeof(t_float4));

			if (vertex[i * 4] < xMin.x)
				memcpy(&xMin, vertex + i * 4, sizeof(t_float4));
			if (vertex[i * 4 + 1] < yMin.y)
				memcpy(&yMin, vertex + i * 4, sizeof(t_float4));
			if (vertex[i * 4 + 2] < zMin.z)
				memcpy(&zMin, vertex + i * 4, sizeof(t_float4));
		}

		float xSegment = fabs(xMax.x - xMin.x);
		float ySegment = fabs(yMax.y - yMin.y);
		float zSegment = fabs(zMax.z - zMin.z);
		GLint e = perspective_projection_matrix(60 * pi / 180., (double)w / (double)h, (t_float2){0.128, 512}, p_uni);
		e = rotation_matrix(0, (t_float4){0, 0, 0, 0}, r_uni);
		e = translation_matrix(-(xSegment / 2 + xMin.x), -(ySegment / 2 + yMin.y), -4 - zSegment, t_uni);
	}

	if (0) {
		*(uint32_t *)object = 1;
		void **obj_ptr = (void *)object + sizeof(uint32_t);
		memset(obj_ptr[0] + MAX_NAME_LEN, 0, sizeof(uint32_t) * 8);
		memcpy(obj_ptr[0] + MAX_NAME_LEN, def_count_total, sizeof(uint32_t) * 4);
	}
	GLuint *arr = wavefront_to_gl_arrays_converter(data, object, &err);*/
	GLint e;
	//e = perspective_projection_matrix(60 * pi / 180., (double)w / (double)h, (t_float2){0.128, 512}, p_uni);
	e = translation_matrix(0, 0, -4, t_uni);
	if (arr == NULL) {
		exit(0);
	}
	/*n_count = arr[1 + n];
	vao_id = arr[1 + arr[0] + n];*/

	return (0);
}

int	main(int argc, char *argv[]) {

	int w = 1024, h = 768;

	// if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
		// exit(0);

	/* Init glfw library */
	if (glfw_init()) {
		return -1;
	}
	/* Init glfw window */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (glfw_init_window(w, h, "GLViewer")) {
		glfwTerminate();
		return -1;
	}
	/* Make window's context current */
 
	glfwMakeContextCurrent(window);
	/*if (glad_init() != 0) {
		glfwTerminate();
		return -1;
	}*/

	t_gl_renderer rndr;
	if (1) {
		char *err = NULL;

		bzero(&rndr, sizeof(t_gl_renderer));
		rndr.canvas = (t_uint2){w, h};
		rndr.fov = 60 * pi / 180.;
		int err_code = gl_renderer_init(&rndr, argv[1], &err);
		if (err_code != 0) {
			printf("%s\n", err);
			bzero(&rndr, sizeof(t_gl_renderer));
			exit(0);
		}
		glUseProgram(rndr.gl.program);
		n_count = *rndr.vtx_array;
		vao_id = *rndr.vao_array;
		//exit(0);
	}

	GLenum err = 0;
	err = glGetError();
	if (err != 0) {
		printf("%d\n", err);
		exit(0);
	}

	//VBO_init();
	//VAO_init();

	/*  */
	/*char *q;
	GLuint program = program_get("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", &q);
	if (!program) {
		printf("%s\n", q);
		free(q);
		exit(0);
	}
	glUseProgram(program);
	err = glGetError();
	if (err != 0) {
		printf("%d\n", err);
		exit(0);
	}

	GLuint	r_uni = glGetUniformLocation(program, "rotation");
	GLuint	p_uni = glGetUniformLocation(program, "projection");
	GLuint	t_uni = glGetUniformLocation(program, "translation");
	func(argv[1], argc == 3 ? (int)ft_atof(argv[2]) : 0, rndr.gl.program, w, h, 0, p_uni, t_uni);
	*/

	double angle = 0;

	double x = 0, y = 0, z = -4;

	glEnable(GL_DEPTH_TEST);

	while(!glfwWindowShouldClose(window)) {
		// double b = glfwGetTime();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			gl_renderer_release(&rndr, 0);
			bzero(&rndr, sizeof(t_gl_renderer));
			break;
		}

		/* Rotation block */
		/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			angle -= M_PI / 180.0;
			rotation_matrix(angle, (t_float4){1, 0, 0, 0}, r_uni);
		} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			angle += M_PI / 180.0;
			rotation_matrix(angle, (t_float4){1, 0, 0, 0}, r_uni);
		} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			angle -= M_PI / 180.0;
			rotation_matrix(angle, (t_float4){0, 1, 0, 0}, r_uni);
		} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			angle += M_PI / 180.0;
			rotation_matrix(angle, (t_float4){0, 1, 0, 0}, r_uni);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			z -= 0.32;
			translation_matrix(0, y, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			z += 0.32;
			translation_matrix(0, y, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			y += 0.32;
			translation_matrix(0, y, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			y -= 0.32;
			translation_matrix(0, y, z, t_uni);
		}*/

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, n_count);
		glBindVertexArray(0);
		err = glGetError();
		if (err != 0) {
			printf("%d\n", err);
			exit(0);
		}

		//glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(t_float4) * n_count/*  + sizeof(t_float2) * n_count */));
		//glDrawArrays(GL_TRIANGLES, 0, n_count);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		// double e = glfwGetTime();
		// printf("FPS: %d, framerate: %f\n", (unsigned int)(1000 / (e - b)), e - b);
	}

	glfwTerminate();
	return 0;
}
