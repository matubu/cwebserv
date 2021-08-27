FLAGS = -pthread -Wall -Wextra -Werror
SRCS = $(wildcard src/*.c)
OUTPUT = $(patsubst src/%.c,bin/%.o,$(SRCS))
NAME = main

bin/%.o: src/%.c
	gcc $(FLAGS) -c src/$*.c -o bin/$*.o
all: $(NAME)
$(NAME): $(OUTPUT)
	gcc $(FLAGS) $(OUTPUT) -o $(NAME)
setup: fclean
	mkdir logs
	mkdir bin
run:
	sudo ./main
clean:
	rm -rf $(NAME)
fclean: clean
	rm -rf $(OUTPUT)
re: fclean all

.PHONY: all clean fclean re
