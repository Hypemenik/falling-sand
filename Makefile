# Ścieżka do SFML (z Homebrew)
GLEW_PATH = /opt/homebrew/Cellar/glew/2.2.0_1
GLFW_PATH = /opt/homebrew/Cellar/glfw/3.4


# Ścieżka do plików źródłowych
cppFileNames := $(shell find ./src -type f -name "*.cpp")

# Kompilator i flagi
CXX = clang++
CXXFLAGS = -std=c++20 -g -I$(GLFW_PATH)/include -I$(GLEW_PATH)/include 
LDFLAGS = -L$(GLEW_PATH)/lib -L$(GLFW_PATH)/lib  -framework OpenGL -lglew  -lglfw

# Domyślna reguła
all: bin/main

bin/main: $(cppFileNames)
	mkdir -p bin
	$(CXX) $(cppFileNames) $(CXXFLAGS) $(LDFLAGS) -o bin/main

clean:
	rm -rf bin