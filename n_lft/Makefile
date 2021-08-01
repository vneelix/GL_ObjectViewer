SRC_P = ./src/

INC_P = ./inc/

OBJ_P = ./obj/

SRC = $(wildcard $(SRC_P)*.c)

INC = $(wildcard $(INC_P)*.h)

OBJ = $(SRC:$(SRC_P)%.c=$(OBJ_P)%.o)

NAME = libft.a

CC = clang

CFLAGS = -Wall -Wextra -Werror -O2 -I$(INC_P) -c

.PHONY: all clean fclean re

$(OBJ_P)%.o: $(SRC_P)%.c $(INC)
	$(CC) $(CFLAGS) $< -o $@

$(NAME): $(OBJ)
	ar rc $(NAME) $^
	ranlib $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
