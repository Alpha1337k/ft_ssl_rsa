NAME=ft_ssl

CC=gcc
FLAGS=-Wall -Werror -Wextra
DEBUG_FLAGS=-g -fsanitize=address
SRC=src/asn1.c \
	src/base64.c \
	src/des.c \
	src/ft_ssl.c \
	src/genrsa.c \
	src/helpers.c \
	src/hexdump.c \
	src/parsers.c \
	src/prime.c \
	src/print.c \
	src/rsa.c \
	src/rsautl.c \
	 -lm
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
