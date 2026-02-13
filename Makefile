# Compiler Settings
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Iinclude -I/opt/local/include/SDL2 -D_THREAD_SAFE
LDFLAGS = -L/opt/local/lib -lSDL2 -lSDL2_ttf -lSDL2_image

# Directories
SRC_DIR = src
BUILD_DIR = build

# Auto-detect all .cpp files in src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Create a list of corresponding .o files in build/
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# The Final Executable Name
TARGET = game_engine

# --- Rules ---

# 1. Link everything together
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 2. Compile each .cpp file into a .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 3. Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: clean