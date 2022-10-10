# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsiguenz <tsiguenz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/09 17:55:43 by tsiguenz          #+#    #+#              #
#    Updated: 2022/10/10 11:26:56 by tsiguenz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
TEST_NAME = $(NAME)_test

CC = c++
FLAGS = -g3 -Wall -Wextra -Werror -std=c++98

SRC_PATH = src/
OBJ_PATH = obj/
TEST_PATH = test/
INC = -Iinclude

SRC_NAME = main.cpp utils.cpp
TEST_SRC_NAME = test_main.cpp
OBJ_NAME = $(SRC_NAME:.cpp=.o)

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
TEST_SRC = $(addprefix $(TEST_PATH), $(TEST_SRC_NAME))

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(INC) $(OBJ) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	mkdir -p $(@D)
	$(CC) $(FLAGS) $(INC) -MMD -c $< -o $@

-include $(OBJ:%.o=%.d)

all: $(NAME)

# Test rule
test:
	$(CC) $(FLAGS) $(INC) -D __IS_TEST__ $(TEST_SRC) $(SRC) -o $(TEST_NAME)

clean:
	rm -rf obj

fclean: clean
	rm -rf $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test
