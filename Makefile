# Ścieżka do SFML (z Homebrew)
SFML_PATH = /opt/homebrew/Cellar/sfml/3.0.1

# Ścieżka do plików źródłowych
cppFileNames := $(shell find ./src -type f -name "*.cpp")

# Kompilator i flagi
CXX = clang++
CXXFLAGS = -std=c++20 -g -I$(SFML_PATH)/include
LDFLAGS = -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system

# Domyślna reguła
all: bin/main

bin/main: $(cppFileNames)
	mkdir -p bin
	$(CXX) $(cppFileNames) $(CXXFLAGS) $(LDFLAGS) -o bin/main

clean:
	rm -rf bin