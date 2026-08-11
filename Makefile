# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/16 15:16:49 by lebeyssa          #+#    #+#              #
#    Updated: 2026/08/11 12:09:18 by lebeyssa         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

CC := cc
CFLAGS := -Wall -Wextra -Werror -pthread -g
INC := -Iheader_file
OBJ_DIR := .objet
SRC := c_file/main.c c_file/simulation.c c_file/moniteur.c c_file/utils_time_1.c\
	c_file/utils_status.c c_file/dongle.c c_file/codeur.c c_file/init.c c_file/clean_up.c\
	c_file/main_utils.c c_file/utils_time_2.c c_file/utils_codeur.c c_file/clean_up_1.c\
	c_file/fn_check.c c_file/init_utils.c c_file/queue_ticker.c c_file/utils_dongles.c\
	c_file/sheduler.c
		
HEADERS = header_file/codexion.h
NAME := codexion 
OBJ := $(patsubst %.c, %.o, $(SRC))
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJ))
GREEN  := \033[32m
RESET   := \033[0m

all : $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GREEN)✓ Compilation done : %s$(RESET)\n" "$(NAME)"
	

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(GREEN)✓ deletion of objects %s$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(GREEN)✓ deletion of the executable : %s$(RESET)\n" "$(NAME)"

re : fclean all

.PHONY : clean fclean re all