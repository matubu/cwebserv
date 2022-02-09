FLAG = -pthread -Wall -Wextra -Werror
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,bin/%.o,$(SRCS))
NAME = main

bin/%.o: src/%.c
	mkdir -p bin
	gcc $(FLAG) -c $^ -o $@
all: $(NAME)
$(NAME): $(OBJS)
	gcc $(FLAG) $(OBJS) -o $(NAME)
update:
	git pull
run: all
	./$(NAME)
clean:
	rm -rf $(NAME)
fclean: clean
	rm -rf $(OBJS)
re: fclean all

.PHONY: all clean fclean re run update
