NAME=ft_ssl

CC=gcc
FLAGS=-Wall -Werror -Wextra
DEBUG_FLAGS=-g -fsanitize=address
LIBS=-I includes
OBJ_DIR=dist
SRC_DIR=src

MAIN=src/ft_ssl.c
SRCS_ALL=$(wildcard $(SRC_DIR)/*.c)
SRCS=$(filter-out $(MAIN),$(SRCS_ALL))
OBJS=$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
RM=rm -rf
LIB=dist/libssl.a

all: $(NAME)

$(NAME): $(LIB) $(OBJS)
	$(CC) $(FLAGS) $(LIBS) $(OBJS) $(MAIN) -lm -o $(NAME)

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(LIBS) $(FLAGS) -c $< -o $@

test: test/base64.c $(LIB)
	gcc test/base64.c $(LIB) $(LIBS) -lm -o base64_test

clean:
	$(RM) $(OBJS) $(LIB)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: re
	./$(NAME)

debug: fclean
	$(CC) $(FLAGS) $(DEBUG_FLAGS) $(LIBS) $(SRC) -o $(NAME)
