# Detect number of cores on macOS (sysctl) or Linux (nproc)
NPROCS = $(shell sysctl -n hw.ncpu 2>/dev/null || nproc)
# Subtract 1 to keep system responsive
JOBS = $(shell echo $$(($(NPROCS) - 1)))

# Override the default "make" command to use those jobs
MAKEFLAGS += -j$(JOBS)

# Compiler Settings
CXX = clang++
# Added -MMD -MP: These generate dependency files (.d) automatically
CXXFLAGS = -std=c++17 -Wall -Iinclude -I/opt/local/include/SDL2 -D_THREAD_SAFE -MMD -MP
LDFLAGS = -L/opt/local/lib -lSDL2 -lSDL2_ttf -lSDL2_image

# Directories
SRC_DIR = src
BUILD_DIR = build

# Auto-detect all .cpp files in src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Create a list of corresponding .o files in build/
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
# Create a list of dependency files (.d)
DEPS = $(OBJS:.o=.d)

# The Final Executable Name
TARGET = game_engine

# --- Rules ---

# 1. Link everything together
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 2. Compile each .cpp file into a .o file
# Now relies on timestamps properly
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 3. Include the dependency files
# This tells make: "If Player.h changes, recompile Player.o too"
-include $(DEPS)

# 4. Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET) logs

.PHONY: clean