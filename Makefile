# Makefile for BlitzMail with Assimp support
# Supports Linux/Unix systems with OpenGL, GLUT, and Assimp

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I. $(shell pkg-config --cflags assimp)
LDFLAGS = -lGL -lGLU -lglut -lm $(shell pkg-config --libs assimp)

# Target executable
TARGET = BlitzMail

# Source files
SOURCES = OpenGL3DTemplate.cpp
HEADERS = ModelLoader.h glut.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential freeglut3-dev libassimp-dev

# Run the program
run: $(TARGET)
	./$(TARGET)

# Help target
help:
	@echo "BlitzMail Makefile"
	@echo "=================="
	@echo "Targets:"
	@echo "  all          - Build the project (default)"
	@echo "  clean        - Remove build artifacts"
	@echo "  run          - Build and run the project"
	@echo "  install-deps - Install required dependencies (Ubuntu/Debian)"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Dependencies:"
	@echo "  - OpenGL"
	@echo "  - GLUT/FreeGLUT"
	@echo "  - Assimp 5.x"
	@echo ""
	@echo "Example usage:"
	@echo "  make install-deps  # Install dependencies"
	@echo "  make               # Build the project"
	@echo "  make run           # Run the project"

.PHONY: all clean install-deps run help
