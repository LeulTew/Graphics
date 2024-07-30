# Compilers
CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -g -Wall
CXXFLAGS = -g -Wall

# Linking flags for libraries
LINKING = -lglut -lGL -lGLU

# Target executable name
TARGET = out

# Source file to compile
# Default to "main.cpp" if not specified
SRC = $(SOURCE).cpp
OBJ = $(SOURCE).o

# Default target
all: $(TARGET)

# Link object file to create the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LINKING)

# Rule for compiling .cpp file to .o file
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up generated files
clean:
	rm -f $(TARGET) $(OBJ)
