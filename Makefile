COLOR_NONE = \e[0m
COLOR_WHITE = \e[1;37m
COLOR_BLUE = \e[1;34m
COLOR_GREEN = \e[1;32m
COLOR_PURPLE = \e[1;35m

SRC_DIR = ./
INC_DIR = ./
OBJ_DIR = obj/

SRCS = 	$(SRC_DIR)change_color_handler.c			$(SRC_DIR)interface_init.c		$(SRC_DIR)renderer.c	\
		$(SRC_DIR)interface_event_handler.c			$(SRC_DIR)rotation_manager.c	$(SRC_DIR)main.c
INCS =	$(INC_DIR)/opengl_project.h
OBJ =	$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))


#minilib
MINILIB_DIR =	minilib
MINILIB_SRC =	$(MINILIB_DIR)/src/error_callback.c		$(MINILIB_DIR)/src/ft_bzero.c	$(MINILIB_DIR)/src/ft_memset.c		$(MINILIB_DIR)/src/ft_strlen.c		\
				$(MINILIB_DIR)/src/ft_atof.c			$(MINILIB_DIR)/src/ft_itoa.c	$(MINILIB_DIR)/src/ft_read_file.c	$(MINILIB_DIR)/src/ft_strsplit.c	\
				$(MINILIB_DIR)/src/ft_atoi.c			$(MINILIB_DIR)/src/ft_memcpy.c	$(MINILIB_DIR)/src/ft_strdup.c		$(MINILIB_DIR)/src/is_number.c

MINILIB_INCS = 	$(MINILIB_DIR)/inc/minilib.h
MINILIB_OBJS = 	$(patsubst $(MINILIB_DIR)/src/%.c,$(OBJ_DIR)%.o,$(MINILIB_SRC))

#gl init
GL_INIT_DIR =	opengl_init
GL_INIT_SRC =	$(GL_INIT_DIR)/gl_init.c		\
				$(GL_INIT_DIR)/matrix_init.c	\
				$(GL_INIT_DIR)/program_get.c	\
				$(GL_INIT_DIR)/shader_get.c		\
				$(GL_INIT_DIR)/uniform_init.c

GL_INIT_INCS = 	$(GL_INIT_DIR)/opengl_init.h
GL_INIT_OBJS = 	$(patsubst $(GL_INIT_DIR)/%.c,$(OBJ_DIR)%.o,$(GL_INIT_SRC))

#renderer init
GL_RENDERER_INIT_DIR =	gl_renderer_init
GL_RENDERER_INIT_SRC =	$(GL_RENDERER_INIT_DIR)/gl_renderer_init.c	\
						$(GL_RENDERER_INIT_DIR)/gl_renderer_util.c	\
						$(GL_RENDERER_INIT_DIR)/gl_renderer_util2.c

GL_RENDERER_INIT_INCS = opengl_project.h
GL_RENDERER_INIT_OBJS = $(patsubst $(GL_RENDERER_INIT_DIR)/%.c,$(OBJ_DIR)%.o,$(GL_RENDERER_INIT_SRC))

#triangulator
TRIANGULATOR_DIR =	triangulator
TRIANGULATOR_SRC =	$(TRIANGULATOR_DIR)/inside_triangle.c	$(TRIANGULATOR_DIR)/stack_op.c		$(TRIANGULATOR_DIR)/util.c				$(TRIANGULATOR_DIR)/wavefront_converter.c \
					$(TRIANGULATOR_DIR)/polygon_handling.c	$(TRIANGULATOR_DIR)/triangulator.c	$(TRIANGULATOR_DIR)/triangulator_3d.c	$(TRIANGULATOR_DIR)/vbo_rewriter.c

TRIANGULATOR_INCS = $(TRIANGULATOR_DIR)/triangulator.h
TRIANGULATOR_OBJS = $(patsubst $(TRIANGULATOR_DIR)/%.c,$(OBJ_DIR)%.o,$(TRIANGULATOR_SRC))

#wavefront reader
WAVEFRONT_OBJECT_READER_DIR =	object_reader
WAVEFRONT_OBJECT_READER_SRC =	$(WAVEFRONT_OBJECT_READER_DIR)/wavefront_object_reader.c	$(WAVEFRONT_OBJECT_READER_DIR)/wavefront_object_writer.c	\
								$(WAVEFRONT_OBJECT_READER_DIR)/definition.c					$(WAVEFRONT_OBJECT_READER_DIR)/wavefront_object_get.c		\
								$(WAVEFRONT_OBJECT_READER_DIR)/number_sequence.c			$(WAVEFRONT_OBJECT_READER_DIR)/wavefront_object_get2.c		\
								$(WAVEFRONT_OBJECT_READER_DIR)/wavefront_def_handler.c

WAVEFRONT_OBJECT_READER_INCS = 	$(WAVEFRONT_OBJECT_READER_DIR)/wavefront_object_reader.h
WAVEFRONT_OBJECT_READER_OBJS = 	$(patsubst $(WAVEFRONT_OBJECT_READER_DIR)/%.c,$(OBJ_DIR)%.o,$(WAVEFRONT_OBJECT_READER_SRC))


#vector
VECTOR_DIR =	vector
VECTOR_SRC =	$(VECTOR_DIR)/matrix_op.c			\
				$(VECTOR_DIR)/op.c					\
				$(VECTOR_DIR)/op_overloadable_0.c	\
				$(VECTOR_DIR)/op_overloadable_1.c

VECTOR_INCS = $(VECTOR_DIR)/vector.h
VECTOR_OBJS = $(patsubst $(VECTOR_DIR)/%.c,$(OBJ_DIR)%.o,$(VECTOR_SRC))

INCL = -I$(INC_DIR) -I$(MINILIB_DIR)/inc -I$(GL_INIT_DIR) -I$(TRIANGULATOR_DIR) -I$(WAVEFRONT_OBJECT_READER_DIR) -I$(VECTOR_DIR)

NAME = 3DViewer

CC = clang
LIBS = `pkg-config --static --libs SDL2 SDL2_image` -framework OpenGL -framework Cocoa
CCFL = -Wall -Wextra -Werror -O2

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ) $(MINILIB_OBJS) $(GL_INIT_OBJS) $(GL_RENDERER_INIT_OBJS) $(WAVEFRONT_OBJECT_READER_OBJS) $(VECTOR_OBJS) $(TRIANGULATOR_OBJS)
	@printf "$(COLOR_GREEN)Compiled successfully$(COLOR_NONE)\n"
	@printf "$(COLOR_GREEN)Linking...$(COLOR_NONE)\n"
	@$(CC) $(OBJ) $(MINILIB_OBJS) $(GL_INIT_OBJS) $(GL_RENDERER_INIT_OBJS) $(WAVEFRONT_OBJECT_READER_OBJS) $(VECTOR_OBJS) $(TRIANGULATOR_OBJS) $(LIBS) -o $(NAME)
	@printf "$(COLOR_GREEN)Built successfully$(COLOR_NONE)\n"

$(OBJ_DIR)%.o:$(SRC_DIR)/%.c $(INCS)
	@printf "$(COLOR_GREEN)Compiling $(COLOR_PURPLE)$<$(COLOR_GREEN) to $(COLOR_PURPLE)$@$(COLOR_GREEN)...$(COLOR_NONE)\n"
	@$(CC) $(CCFL) $(INCL) $< -c -o $@

$(OBJ_DIR)%.o:$(MINILIB_DIR)/src/%.c $(MINILIB_INCS)
	@printf "$(COLOR_GREEN)Compiling $(COLOR_PURPLE)$<$(COLOR_GREEN) to $(COLOR_PURPLE)$@$(COLOR_GREEN)...$(COLOR_NONE)\n"
	@$(CC) $(CCFL) $(INCL) $< -c -o $@

$(OBJ_DIR)%.o:$(GL_INIT_DIR)/%.c $(GL_INIT_INCS)
	@printf "$(COLOR_GREEN)Compiling $(COLOR_PURPLE)$<$(COLOR_GREEN) to $(COLOR_PURPLE)$@$(COLOR_GREEN)...$(COLOR_NONE)\n"
	@$(CC) $(CCFL) $(INCL) $< -c -o $@

$(OBJ_DIR)%.o:$(GL_RENDERER_INIT_DIR)/%.c $(GL_RENDERER_INIT_INCS)
	@printf "$(COLOR_GREEN)Compiling $(COLOR_PURPLE)$<$(COLOR_GREEN) to $(COLOR_PURPLE)$@$(COLOR_GREEN)...$(COLOR_NONE)\n"
	@$(CC) $(CCFL) $(INCL) $< -c -o $@

$(OBJ_DIR)%.o:$(TRIANGULATOR_DIR)/%.c $(TRIANGULATOR_INCS)
	@printf "$(COLOR_GREEN)Compiling $(COLOR_PURPLE)$<$(COLOR_GREEN) to $(COLOR_PURPLE)$@$(COLOR_GREEN)...$(COLOR_NONE)\n"
	@$(CC) $(CCFL) $(INCL) $< -c -o $@

$(OBJ_DIR)%.o:$(WAVEFRONT_OBJECT_READER_DIR)/%.c $(WAVEFRONT_OBJECT_READER_INCS)
	@printf "$(COLOR_GREEN)Compiling $(COLOR_PURPLE)$<$(COLOR_GREEN) to $(COLOR_PURPLE)$@$(COLOR_GREEN)...$(COLOR_NONE)\n"
	@$(CC) $(CCFL) $(INCL) $< -c -o $@

$(OBJ_DIR)%.o:$(VECTOR_DIR)/%.c $(VECTOR_INCS)
	@printf "$(COLOR_GREEN)Compiling $(COLOR_PURPLE)$<$(COLOR_GREEN) to $(COLOR_PURPLE)$@$(COLOR_GREEN)...$(COLOR_NONE)\n"
	@$(CC) $(CCFL) $(INCL) $< -c -o $@

clean:
	@printf "$(COLOR_GREEN)Cleaning object files...$(COLOR_NONE)\n"
	@/bin/rm -rf $(OBJ) $(MINILIB_OBJS) $(GL_INIT_OBJS) $(GL_RENDERER_INIT_OBJS) $(WAVEFRONT_OBJECT_READER_OBJS) $(VECTOR_OBJS) $(TRIANGULATOR_OBJS)
	@printf "$(COLOR_GREEN)Cleaned successfully$(COLOR_NONE)\n"

fclean: clean
	@printf "$(COLOR_GREEN)Cleaning executable...$(COLOR_NONE)\n"
	@/bin/rm -f $(NAME)

re: fclean all
