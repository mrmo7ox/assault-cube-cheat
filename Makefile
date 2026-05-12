NAME    = cheat
CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++11

SRC     = cheat.cpp
OBJ     = $(SRC:.cpp=.o)
HDR     = player.hpp vector3.hpp cheat.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HDR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re