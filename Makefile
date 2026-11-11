# Makefile for Memory Allocation Simulator
# Compatible with GNU Make (Ubuntu/WSL and NetBSD with gmake)
# Standard: C99, Platform: Linux/Unix

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -O2 -g -D_POSIX_C_SOURCE=199309L
LDFLAGS = 

# Directories
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
ALGO_DIR = $(SRC_DIR)/algorithms
UI_DIR = $(SRC_DIR)/ui
UTILS_DIR = $(SRC_DIR)/utils
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Output binary
TARGET = memory_allocator

# Source files
CORE_SRCS = $(CORE_DIR)/algorithm_type.c \
            $(CORE_DIR)/memory_block.c \
            $(CORE_DIR)/heap_manager.c

ALGO_SRCS = $(ALGO_DIR)/first_fit.c \
            $(ALGO_DIR)/next_fit.c \
            $(ALGO_DIR)/best_fit.c \
            $(ALGO_DIR)/worst_fit.c

UI_SRCS = $(UI_DIR)/cli.c \
          $(UI_DIR)/visualizer.c \
          $(UI_DIR)/shell.c

UTILS_SRCS = $(UTILS_DIR)/input_utils.c \
             $(UTILS_DIR)/shell_parser.c \
             $(UTILS_DIR)/benchmark.c \
             $(UTILS_DIR)/export.c

MAIN_SRC = $(SRC_DIR)/main.c

SRCS = $(CORE_SRCS) $(ALGO_SRCS) $(UI_SRCS) $(UTILS_SRCS) $(MAIN_SRC)

# Object files
OBJS = $(OBJ_DIR)/algorithm_type.o \
       $(OBJ_DIR)/memory_block.o \
       $(OBJ_DIR)/heap_manager.o \
       $(OBJ_DIR)/first_fit.o \
       $(OBJ_DIR)/next_fit.o \
       $(OBJ_DIR)/best_fit.o \
       $(OBJ_DIR)/worst_fit.o \
       $(OBJ_DIR)/cli.o \
       $(OBJ_DIR)/visualizer.o \
       $(OBJ_DIR)/shell.o \
       $(OBJ_DIR)/input_utils.o \
       $(OBJ_DIR)/shell_parser.o \
       $(OBJ_DIR)/benchmark.o \
       $(OBJ_DIR)/export.o \
       $(OBJ_DIR)/main.o

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build successful! Executable: $(TARGET)"

# Compile core source files
$(OBJ_DIR)/algorithm_type.o: $(CORE_DIR)/algorithm_type.c $(CORE_DIR)/algorithm_type.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/memory_block.o: $(CORE_DIR)/memory_block.c $(CORE_DIR)/memory_block.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/heap_manager.o: $(CORE_DIR)/heap_manager.c $(CORE_DIR)/heap_manager.h \
                            $(CORE_DIR)/memory_block.h $(CORE_DIR)/algorithm_type.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile algorithm source files
$(OBJ_DIR)/first_fit.o: $(ALGO_DIR)/first_fit.c $(ALGO_DIR)/first_fit.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/next_fit.o: $(ALGO_DIR)/next_fit.c $(ALGO_DIR)/next_fit.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/best_fit.o: $(ALGO_DIR)/best_fit.c $(ALGO_DIR)/best_fit.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/worst_fit.o: $(ALGO_DIR)/worst_fit.c $(ALGO_DIR)/worst_fit.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile UI source files
$(OBJ_DIR)/cli.o: $(UI_DIR)/cli.c $(UI_DIR)/cli.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/visualizer.o: $(UI_DIR)/visualizer.c $(UI_DIR)/visualizer.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/shell.o: $(UI_DIR)/shell.c $(UI_DIR)/shell.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile utils source files
$(OBJ_DIR)/input_utils.o: $(UTILS_DIR)/input_utils.c $(UTILS_DIR)/input_utils.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/shell_parser.o: $(UTILS_DIR)/shell_parser.c $(UTILS_DIR)/shell_parser.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/benchmark.o: $(UTILS_DIR)/benchmark.c $(UTILS_DIR)/benchmark.h $(CORE_DIR)/heap_manager.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/export.o: $(UTILS_DIR)/export.c $(UTILS_DIR)/export.h $(CORE_DIR)/heap_manager.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main source file
$(OBJ_DIR)/main.o: $(MAIN_SRC) $(CORE_DIR)/heap_manager.h $(CORE_DIR)/algorithm_type.h \
                    $(UI_DIR)/cli.h $(UI_DIR)/shell.h $(UTILS_DIR)/benchmark.h | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory if it doesn't exist
$(OBJ_DIR):
	@echo "Creating build directories..."
	@mkdir -p $(OBJ_DIR)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(TARGET) $(TEST_AC_TARGET) $(TEST_P2_TARGET)
	@echo "Clean complete!"

# Run the program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

# Debug build with more verbose output
debug: CFLAGS += -DDEBUG -O0 -g3
debug: clean all

# Release build with optimizations
release: CFLAGS += -O3 -DNDEBUG
release: clean all

# Check for memory leaks with Valgrind (Ubuntu/WSL only)
valgrind: $(TARGET)
	@echo "Running Valgrind memory leak check..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Test targets
TEST_DIR = tests
TEST_AC_TARGET = test_acceptance_criteria
TEST_P2_TARGET = test_phase2_features
TEST_OBJS = $(OBJ_DIR)/algorithm_type.o \
            $(OBJ_DIR)/memory_block.o \
            $(OBJ_DIR)/heap_manager.o \
            $(OBJ_DIR)/first_fit.o \
            $(OBJ_DIR)/next_fit.o \
            $(OBJ_DIR)/best_fit.o \
            $(OBJ_DIR)/worst_fit.o

TEST_P2_OBJS = $(TEST_OBJS) \
               $(OBJ_DIR)/visualizer.o

# Build AC test suite (without UI/main.o)
test: $(TEST_OBJS) | $(OBJ_DIR)
	@echo "Building acceptance criteria test suite..."
	$(CC) $(CFLAGS) $(TEST_DIR)/test_acceptance_criteria.c $(TEST_OBJS) -o $(TEST_AC_TARGET)
	@echo "Test executable created: $(TEST_AC_TARGET)"

# Build Phase 2 test suite
test-phase2: $(TEST_P2_OBJS) | $(OBJ_DIR)
	@echo "Building Phase 2 features test suite..."
	$(CC) $(CFLAGS) $(TEST_DIR)/test_phase2_features.c $(TEST_P2_OBJS) -o $(TEST_P2_TARGET)
	@echo "Test executable created: $(TEST_P2_TARGET)"

# Run AC test suite
run-tests: test
	@echo "Running acceptance criteria tests..."
	@./$(TEST_AC_TARGET)

# Run Phase 2 test suite
run-tests-phase2: test-phase2
	@echo "Running Phase 2 feature tests..."
	@./$(TEST_P2_TARGET)

# Run all tests
run-all-tests: test test-phase2
	@echo "Running all test suites..."
	@echo ""
	@echo "=== ACCEPTANCE CRITERIA TESTS ==="
	@./$(TEST_AC_TARGET)
	@echo ""
	@echo "=== PHASE 2 FEATURE TESTS ==="
	@./$(TEST_P2_TARGET)

# Run AC tests with Valgrind
test-valgrind: test
	@echo "Running AC tests with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_AC_TARGET)

# Run Phase 2 tests with Valgrind
test-valgrind-phase2: test-phase2
	@echo "Running Phase 2 tests with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_P2_TARGET)

# Display help
help:
	@echo "Memory Allocation Simulator - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build the project (default)"
	@echo "  clean            - Remove build artifacts"
	@echo "  run              - Build and run the program"
	@echo "  debug            - Build with debug symbols (-g3 -O0)"
	@echo "  release          - Build with optimizations (-O3)"
	@echo "  valgrind         - Run with Valgrind memory leak detection"
	@echo "  test             - Build acceptance criteria test suite"
	@echo "  test-phase2      - Build Phase 2 features test suite"
	@echo "  run-tests        - Run AC tests"
	@echo "  run-tests-phase2 - Run Phase 2 tests"
	@echo "  run-all-tests    - Run all test suites"
	@echo "  test-valgrind    - Run AC tests with Valgrind"
	@echo "  test-valgrind-phase2 - Run Phase 2 tests with Valgrind"
	@echo "  help             - Display this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make                      # Build the project"
	@echo "  make clean                # Clean build files"
	@echo "  make run                  # Build and run interactive CLI"
	@echo "  make run-tests-phase2     # Test Phase 2 features"
	@echo "  make run-all-tests        # Run all tests"

# Phony targets (not actual files)
.PHONY: all clean run debug release valgrind test test-phase2 run-tests run-tests-phase2 run-all-tests test-valgrind test-valgrind-phase2 help
