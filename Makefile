NAME	= minishell

CC		= gcc

FLAGS	= -Wall -Wextra -Werror -g #-fsanitize=address

INCLUDE	= include

SRC_PATH	= src

OBJ_PATH	= obj

SOURCES =	mini.c \
			parsing.c \
			parsing_utils.c \
			ft_split_parsing.c \
			ft_split_parsing_pipe.c \
			ft_split_parsing_quote.c \
			ft_split_parsing_quote_bis.c \
			parsing_var_env.c \
			parsing_charset.c \
			parsing_count.c \
			parsing_utils_create.c \
			parsing_utils_str.c \
			parsing_check.c \
			parsing_fill.c \
			parsing_found.c \
			parsing_file.c \
			parsing_init.c \
			ft_split.c \
			ft_strjoin.c \
			lst.c \
			utils.c \
			ft_str.c \
			export.c \
			env.c \
			pwd.c \
			print.c \
			echo.c \
			cd.c \
			cd_handle.c \
			unset.c \
			exit.c \
			pipex.c \
			error.c \
			exec.c \
			signal.c \
			signal_handle.c \
			heredoc.c \
			free.c

SRC	= $(addprefix $(SRC_PATH)/,$(SOURCES))

OBJ	= $(addprefix $(OBJ_PATH)/,$(SOURCES:.c=.o))

NOC		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
WHITE	= \033[1;37m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) -o $@ $^ -lreadline
	@echo "$(GREEN)$@$(NOC)"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDE)/$(NAME).h
	@mkdir -p obj
	@$(CC) $(FLAGS) -I$(INCLUDE) -c -o $@ $<
	@echo "$(BLUE)gcc $(WHITE)$(notdir $@)$(NOC)"

clean:
	@echo "$(RED)clean$(NOC)"
	@rm -rf $(OBJ_PATH)

fclean: clean
	@echo "$(RED)fclean$(NOC)"
	@rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
