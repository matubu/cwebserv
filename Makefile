FLAG = -pthread -Wall -Wextra -Werror
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,bin/%.o,$(SRCS))
NAME = main

bin/%.o: src/%.c
	gcc $(FLAG) -c $^ -o $@
all: $(NAME)
$(NAME): $(OBJS)
	gcc $(FLAG) $(OBJS) -o $(NAME)
setup: fclean
	mkdir logs
	mkdir bin
update:
	git pull
run: all
	sudo ./main
updaterun: update run
clean:
	rm -rf $(NAME)
fclean: clean
	rm -rf $(OBJS)
re: fclean all

.PHONY: setup all clean fclean re run updaterun
