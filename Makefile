DEF_COLOR     = \033[0;39m
GRAY         = \033[0;90m
RED         = \033[0;91m
GREEN         = \033[0;92m
YELLOW         = \033[0;93m
BLUE         = \033[0;94m
MAGENTA     = \033[0;95m
CYAN         = \033[0;96m
WHITE         = \033[0;97m
NAME		= robot

CC			= gcc

SRCS_DIR	= srcs/
OBJS_DIR	= objs/

INCLUDES	= -I includes/

CFLAGS	= -Wall -Werror -Wextra $(INCLUDES) -fsanitize=leak -g3

SRC_FILES	= main\
				grid\
				print\
				robot\
				convert_path/convert_path_to_actions\
				BFS/BFS\
				add_dropoff_path\
				Path_Planning\

SRCS	= $(addprefix $(SRCS_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJS	= $(addprefix $(OBJS_DIR), $(addsuffix .o, $(SRC_FILES)))

all		:
		@mkdir -p $(OBJS_DIR)
		@echo "$(MAGENTA) --------------------------INSIDE SO_LONG-------------------------- $(CYAN)"
		@echo "\n"
		make $(NAME)
		@echo "$(YELLOW)Done Creating ./robot$(DEF_COLOR)"
		@echo "\n"
		@echo "$(GREEN) -------------$(RED)I $(CYAN)AM $(GREEN)DONE $(YELLOW)MADAFAKA$(GREEN)-------------------------- $(DEF_COLOR)"

# Modified pattern rule to handle subdirectories
$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
		@mkdir -p $(@D)
		@$(CC) $(CFLAGS) -Wall -Wextra -Werror -O3 -c $< -o $@
		@echo "$(GREEN)Compiling $<$(DEF_COLOR)"

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lm


clean	:
		rm -rf $(OBJS_DIR)

fclean	:	clean
		rm -f $(NAME)

re		:fclean all

.PHONY	:all clean fclean re