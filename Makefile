# Makefile for BlitzMail with Assimp support
# Supports Linux build

CXX = g++
CXXFLAGS = -Wall -std=c++11 -I. -Iexternal/assimp-install/include
LDFLAGS = -Lexternal/assimp-install/lib -lGL -lGLU -lglut -lassimp -lm -Wl,-rpath,external/assimp-install/lib

TARGET = BlitzMail
SOURCES = OpenGL3DTemplate.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run with ./$(TARGET)"

%.o: %.cpp ModelLoader.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

help:
	@echo "Available targets:"
	@echo "  all   - Build the project (default)"
	@echo "  clean - Remove build artifacts"
	@echo "  run   - Build and run the project"
	@echo "  help  - Show this help message"
