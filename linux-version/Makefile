NAME    = cheat
CXX      = c++
INCLUDES= -Iimgui -Iimgui/backends
CXXFLAGS   = -Wall -Wextra -Werror -std=c++11 $(INCLUDES)
LDFLAGS = -lX11 -lglfw -lGL -lpthread -ldl

SRC     = cheat.cpp \
          main.cpp \
          imgui/imgui.cpp \
          imgui/imgui_draw.cpp \
          imgui/imgui_widgets.cpp \
          imgui/imgui_tables.cpp \
          imgui/backends/imgui_impl_glfw.cpp \
          imgui/backends/imgui_impl_opengl3.cpp

OBJ     = $(SRC:.cpp=.o)
HDR     = player.hpp vector3.hpp cheat.hpp mem.hpp vec2.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.cpp $(HDR)
	$(CXX) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re