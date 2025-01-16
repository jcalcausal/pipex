CC = cc
FLAGS = -Wall -Wextra -Werror
RM = rm -rf

NAME = pipex
SRC_DIR = src/
SRC = pipex.c
OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRC:.c=.o))


all: libft $(NAME)


$(NAME): $(OBJS)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS) libft/*.o
	@echo " ... pipex compiled ..."

$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(FLAGS) -c $< -o $@

libft:
	@make -C libft/

clean:
	@$(RM) -r $(OBJS_DIR)
	@make -C libft/ clean
	@echo " ... Clean finished ..."

fclean: clean
	@$(RM) $(NAME)
	@make -C libft/ fclean

re: fclean all

.PHONY: all bonus libft clean fclean re