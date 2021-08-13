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

int	main(int argc, char *argv[]) {

	int w = 1280, h = 800;

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

	GLenum err_opengl_enum = 0;

	/*GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	err_opengl_enum = glGetError();
	if (err_opengl_enum != 0) {
		printf("%d\n", err_opengl_enum);
		exit(0);
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

		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
			exit(0);
		/*SDL_Surface *s_img = IMG_Load("image.jpeg");
		SDL_Surface *s_img_ptr = SDL_ConvertSurfaceFormat(s_img, SDL_PIXELFORMAT_RGBA32, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_img_ptr->w, s_img_ptr->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s_img_ptr->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		err_opengl_enum = glGetError();
		if (err_opengl_enum != 0) {
			printf("%d\n", err_opengl_enum);
			exit(0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);*/
	}

	err_opengl_enum = glGetError();
	if (err_opengl_enum != 0) {
		printf("%d\n", err_opengl_enum);
		exit(0);
	}

	glEnable(GL_DEPTH_TEST);

	while(!glfwWindowShouldClose(window)) {
		// double b = glfwGetTime();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			gl_renderer_release(&rndr, 0);
			bzero(&rndr, sizeof(t_gl_renderer));
			break;
		}

		/* Rotation block */
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			t_matrix4x4	ry = rotation_manager(-1, 0);
			glUniformMatrix4fv(rndr.gl.rotation_matrix, 1, GL_TRUE, (const GLfloat *)&ry);
		} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			t_matrix4x4	ry = rotation_manager(+1, 0);
			glUniformMatrix4fv(rndr.gl.rotation_matrix, 1, GL_TRUE, (const GLfloat *)&ry);
		} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			t_matrix4x4	ry = rotation_manager(0, -1);
			glUniformMatrix4fv(rndr.gl.rotation_matrix, 1, GL_TRUE, (const GLfloat *)&ry);
		} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			t_matrix4x4	ry = rotation_manager(0, +1);
			glUniformMatrix4fv(rndr.gl.rotation_matrix, 1, GL_TRUE, (const GLfloat *)&ry);
		}

		/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
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

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rndr.gl.texture);
		glDrawArrays(GL_TRIANGLES, 0, n_count);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		err_opengl_enum = glGetError();
		if (err_opengl_enum != 0) {
			printf("%d\n", err_opengl_enum);
			exit(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		// double e = glfwGetTime();
		// printf("FPS: %d, framerate: %f\n", (unsigned int)(1000 / (e - b)), e - b);
	}

	glfwTerminate();
	return 0;
}
