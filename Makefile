# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I src
LDFLAGS := -lglfw -lGL -ldl

# Source and build directories
SRC_DIR := src
OBJ_DIR := obj
OBJ_DIR_MODULES := $(OBJ_DIR)/calibrator

3RDPARTY_SRCS := thirdparty/glad/src/
3RDPARTY_DIRS := thirdparty/glad/include/

BIN := raspiance

# Source files and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS)) $(OBJ_DIR)/gl.o

# Default target
all: $(BIN)

# Linking
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I $(3RDPARTY_DIRS) -c $< -o $@

# Compile libs
$(OBJ_DIR)/%.o: $(3RDPARTY_SRCS)/%.c | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I $(3RDPARTY_DIRS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) $(OBJ_DIR_MODULES)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN)

# Run the program
run: $(BIN)
	./$(BIN)

build: $(BIN)

.PHONY: all clean run
