# Compiler settings
CC = clang
CFLAGS = -Wall -Wextra -g -std=c11
LDFLAGS =

# Directory settings
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = $(SRC_DIR)/tests

# Find all source and header files
SRCS = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)

# Generate object file names
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Main executable name
TARGET = $(BUILD_DIR)/lox

# Test sources and executable
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(TEST_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_TARGET = $(BUILD_DIR)/test_runner

# Default target
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Build and run
run: $(TARGET)
	./$(TARGET)

# Build tests
$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) $(LDFLAGS) -o $@

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Declare phony targets
.PHONY: all run test clean


