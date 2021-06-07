#include "opengl_project.h"

GLFWwindow *window = null;

GLfloat vertex_buffer[] = {
	-1,  1, 0,
	 1,  1, 0,
	 1, -1, 0,
	-1, -1, 0
};

GLfloat color_buffer[] = {
	1, 0, 0, 1,
	0, 1, 0, 1,
	0, 0, 1, 1,
	1, 0, 0, 1
};

/* 0 1 2 3 {0 1 2} */

GLuint index_buffer[] = {
	0, 1, 2, 2, 3, 0
	// 0, 3, 2
};

float normal_buffer[] = {
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0
};

GLuint VBO[4];

GLuint VAO_id = 0;

int	VBO_init() {
	/* Get buffer object id's */
	glGenBuffers(sizeof(VBO) / sizeof(GLuint), VBO);
	/* bind vertex buffer object */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	/* Write data into buffer object */
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);
	/* Unbind vertex buffer */
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer), color_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer), index_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer), normal_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

int VAO_init() {
	/* Gen vertax array's list */
	glGenVertexArrays(1, &VAO_id);

	glBindVertexArray(VAO_id);

	

	/* Init coord buffer in VAO */
	glEnableVertexArrayAttrib(VAO_id, 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, null);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Init color buffer in VAO */
	glEnableVertexArrayAttrib(VAO_id, 1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, null);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Init normal buffer in VAO */
	glEnableVertexArrayAttrib(VAO_id, 2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, null);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

int	main(int argc, char *argv[]) {

	/* Init glfw library */
	if (glfw_init()) {
		return -1;
	}
	/* Init glfw window */
	if (glfw_init_window(1024, 768, "Title")) {
		glfwTerminate();
		return -1;
	}
	/* Make window's context current */
	glfwMakeContextCurrent(window);

	if (glad_init() != 0) {
		glfwTerminate();
		return -1;
	}

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
	perspective_matrix(60 * pi / 180., 4. / 3., 2, 256, p_uni);
	rotation_matrix(0, 0, 0, 0, r_uni);
	tranlsation_matrix(0, 0, -4, t_uni);
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
			z -= 1;
			tranlsation_matrix(0, 0, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			z += 1;
			tranlsation_matrix(0, 0, z, t_uni);
		} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			angle -= M_PI / 180.0;
			rotation_matrix(angle, 0, 1, 0, r_uni);
		} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			angle += M_PI / 180.0;
			rotation_matrix(angle, 0, 1, 0, r_uni);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[2]);
		glDrawElements(GL_TRIANGLES, sizeof(index_buffer) / sizeof(float), GL_UNSIGNED_INT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		// double e = glfwGetTime();
		// printf("FPS: %d, framerate: %f\n", (unsigned int)(1000 / (e - b)), e - b);
	}

	glfwTerminate();
	return 0;
}
