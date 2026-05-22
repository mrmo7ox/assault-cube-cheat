NAME    = cheat.exe
CXX     = g++
INCLUDES= -Iimgui -Iimgui/backends -I./glfw/include
CXXFLAGS = -Wall -Wextra -std=c++11 $(INCLUDES)
LDFLAGS = -L./glfw/lib -lglfw3 -lopengl32 -lgdi32 -limm32 -luser32 -lshell32

SRC     = cheat.cpp \
          main.cpp \
          imgui/imgui.cpp \
          imgui/imgui_draw.cpp \
          imgui/imgui_widgets.cpp \
          imgui/imgui_tables.cpp \
          imgui/backends/imgui_impl_glfw.cpp \
          imgui/backends/imgui_impl_opengl3.cpp

OBJ     = $(SRC:.cpp=.o)
HDR     = player.hpp vector3.hpp cheat.hpp mem.hpp vector2.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re