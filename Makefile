CC = cc
FLAGS = -Wall -Wextra -Werror
RM = rm -rf

NAME = pipex
SRC_DIR = src/
SRC = pipex.c
OBJ_DIR = objs/
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))


all: libft $(NAME)

bonus: libft $(NAME_B)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) libft/*.o
	@echo " ... Pipex compiled ..."

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(FLAGS) -c $< -o $@

libft:
	@make -C libft/

clean:
	@$(RM) -r $(OBJ_DIR)
	@make -C libft/ clean
	@echo " ... Clean finished ..."

fclean: clean
	@$(RM) $(NAME)
	@make -C libft/ fclean

re: fclean all

.PHONY: all bonus libft clean fclean re