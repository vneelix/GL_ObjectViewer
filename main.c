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

/*int glad_init() {
	if (gladLoadGL(glfwGetProcAddress) == 0) {
		char *message = "An error occurred during load opengl functions\n";
		write(2, message, strlen(message));
		return -1;
	}
	return 0;
}*/

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

int func(const char *path, int n) {
	void	**data;

	char *err;

	void **object;
	uint32_t	def_count_total[4];

	data = (void **)wavefront_object_reader(path, &object, def_count_total, &err);

	if (!data) {
		printf("%s\n", err);
		exit(0);
	}

	GLuint *arr = wavefront_to_gl_arrays_converter(data, def_count_total, object, NULL);
	if (arr == NULL)
		exit(0);
	n_count = arr[1 + n];
	vao_id = arr[1 + arr[0] + n];
	return (0);

	uint32_t obj_count = *(uint32_t *)object;

	float *vb = NULL;

	for (int i = 0; i != 1; i++) {
		void *ptr = ((void **)((void *)object + sizeof(uint32_t)))[n];
		uint32_t	*def_count = ptr + MAX_NAME_LEN;
		uint32_t	*offset = def_count + 4;

		uint32_t	t0[4], t1[4];
		memcpy(t0, def_count, sizeof(uint32_t) * 4);
		memcpy(t1, offset, sizeof(uint32_t) * 4);

		vb = wavefront_to_gl_vbo_converter(data, def_count);
	}

	if (vb == NULL)
		exit(0);

	n_count = *vb;

	glGenVertexArrays(1, &vao_id);
	glGenBuffers(1, &vbo_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_float4) * *vb * 2, vb + 1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao_id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(sizeof(t_float4) * n_count));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	free(vb);

	/* float *normal_ptr = (void *)vb + sizeof(float) + sizeof(t_float4) * n_count + sizeof(t_float2) * n_count;
	for (int i = 0; i != n_count; i++) {
		printf("% f % f % f % f\n", normal_ptr[0], normal_ptr[1], normal_ptr[2], normal_ptr[3]);
		normal_ptr += 4;
	} */

	return (0);
}

void error_callback_111(int code, const char* description)
{
    printf("%d %s\n", code, description);
}

int	main(int argc, char *argv[]) {

	int ww = 1024, hh = 768;

	// if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
		// exit(0);

	/* Init glfw library */
	if (glfw_init()) {
		return -1;
	}
	glfwSetErrorCallback(error_callback_111);
	/* Init glfw window */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (glfw_init_window(ww, hh, "GLViewer")) {
		glfwTerminate();
		return -1;
	}
	/* Make window's context current */
 
	glfwMakeContextCurrent(window);
	/*if (glad_init() != 0) {
		glfwTerminate();
		return -1;
	}*/

	GLenum err = 0;
	err = glGetError();
	if (err != 0) {
		printf("%d\n", err);
		exit(0);
	}

	//VBO_init();
	//VAO_init();

	/*  */
	GLuint program = create_program();
	if (!program) {
		exit(0);
	}
	glUseProgram(program);
	err = glGetError();
	if (err != 0) {
		printf("%d\n", err);
		exit(0);
	}

	GLuint r_uni = glGetUniformLocation(program, "rotation");
	GLuint p_uni = glGetUniformLocation(program, "projection");
	GLuint t_uni = glGetUniformLocation(program, "translation");
	err = glGetError();
		if (err != 0) {
			printf("%d\n", err);
			exit(0);
		}

	perspective_matrix(60 * pi / 180., (double)ww / (double)hh, 0.128, 2048, p_uni);
	rotation_matrix(0, 0, 0, 0, r_uni);
	tranlsation_matrix(0, 0, -4, t_uni);
	err = glGetError();
	if (err != 0) {
		printf("%d\n", err);
		exit(0);
	}

	func(argv[1], argc == 3 ? (int)ft_atof(argv[2]) : 0);

	double angle = 0;

	double x = 0, y = 0, z = -4;

	glEnable(GL_DEPTH_TEST);

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
