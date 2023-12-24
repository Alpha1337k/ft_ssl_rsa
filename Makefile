NAME=ft_ssl

CC=gcc
FLAGS=
# FLAGS=-Wall -Werror -Wextra -Wno-unused-variable
DEBUG_FLAGS=-g -fsanitize=address
SRC=src/*.c
LIBS=-I includes
OBJ=
RM =rm -rf

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) $(LIBS) $(SRC) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: re
	./$(NAME)

debug: fclean
	$(CC) $(FLAGS) $(DEBUG_FLAGS) $(LIBS) $(SRC) -o $(NAME)
