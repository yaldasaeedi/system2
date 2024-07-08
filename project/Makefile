# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Source and header files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
INC_FILES = $(wildcard $(INC_DIR)/*.h)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Output executable
TARGET = $(BIN_DIR)/real_time_system

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ_FILES)
	@echo "Linking..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful. Executable: $(TARGET)"

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_FILES)
	@echo "Compiling $<..."
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean up build files
clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete."

.PHONY: all clean
