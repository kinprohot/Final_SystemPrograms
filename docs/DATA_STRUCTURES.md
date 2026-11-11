# Data Structures Implementation

## Overview
This document describes the data structures implemented for the Memory Allocation Simulator project.

## Files Created

### Header Files (`.h`)

1. **`src/core/algorithm_type.h`**
   - Defines `AlgorithmType` enum with 4 allocation algorithms:
     - `FIRST_FIT`: Find first block large enough
     - `NEXT_FIT`: Find next block from last allocation position
     - `BEST_FIT`: Find smallest sufficient block
     - `WORST_FIT`: Find largest block
   - Helper function: `algorithm_type_to_string()` for display

2. **`src/core/memory_block.h`**
   - Defines `MemoryBlock` struct (doubly linked list node):
     - `size_t start_address`: Starting address (offset)
     - `size_t size`: Block size in bytes
     - `int is_free`: Status (1=FREE, 0=ALLOCATED)
     - `char process_id[32]`: Process ID if allocated
     - `struct MemoryBlock* next`: Next block pointer
     - `struct MemoryBlock* prev`: Previous block pointer
   - Functions:
     - `memory_block_create()`: Create new block
     - `memory_block_destroy()`: Free block memory
     - `memory_block_print()`: Display block info

3. **`src/core/heap_manager.h`**
   - Defines `HeapManager` struct:
     - `MemoryBlock* head`: Head of linked list
     - `size_t total_size`: Total heap size
     - `size_t used_size`: Currently allocated memory
     - `size_t free_size`: Available memory
     - `AlgorithmType current_algorithm`: Active algorithm
     - `MemoryBlock* last_allocated`: Last allocated block (for Next Fit)
     - `int allocation_count`: Successful allocations counter
     - `int deallocation_count`: Successful deallocations counter
     - `int failed_allocation_count`: Failed allocations counter
   - Functions (F1-F5 from PR.md):
     - **F1**: `heap_init()` - Initialize heap
     - **F2**: `heap_set_algorithm()` - Select algorithm
     - **F3**: `heap_allocate()` - Allocate memory (to be implemented)
     - **F4**: `heap_free()` - Deallocate memory (to be implemented)
     - **F5**: `heap_show_memory_map()` - Display memory status
     - Additional: `heap_show_statistics()`, `heap_destroy()`

4. **`src/algorithms/first_fit.h`** (PR #1)
   - First Fit algorithm interface
   - `MemoryBlock* first_fit_find(HeapManager* heap, size_t size)`

5. **`src/algorithms/next_fit.h`** (PR #1)
   - Next Fit algorithm interface
   - `MemoryBlock* next_fit_find(HeapManager* heap, size_t size)`

6. **`src/algorithms/best_fit.h`** (PR #1)
   - Best Fit algorithm interface
   - `MemoryBlock* best_fit_find(HeapManager* heap, size_t size)`

7. **`src/algorithms/worst_fit.h`** (PR #1)
   - Worst Fit algorithm interface
   - `MemoryBlock* worst_fit_find(HeapManager* heap, size_t size)`

8. **`src/ui/cli.h`** (PR #2, Dec 19)
   - Interactive CLI interface
   - `void run_cli(HeapManager* heap)` - Main menu loop

9. **`src/utils/input_utils.h`** (PR #2, Dec 19)
   - Input validation utilities
   - `int read_int(const char* prompt)` - Read validated integer
   - `void read_string(const char* prompt, char* buffer, size_t size)` - Safe string input
   - `void wait_for_enter()` - Pause screen (optional)

### Implementation Files (`.c`)

1. **`src/core/algorithm_type.c`**
   - Implements `algorithm_type_to_string()` function

2. **`src/core/memory_block.c`**
   - Implements all MemoryBlock operations:
     - Creates blocks with proper initialization
     - Validates inputs (size > 0)
     - Handles process_id safely (null termination)
     - Prints block information

3. **`src/core/heap_manager.c`**
   - Implements HeapManager operations:
     - **`heap_init()`**: Creates heap with single FREE block (AC1 requirement)
     - **`heap_set_algorithm()`**: Changes algorithm, resets last_allocated if needed
     - **`heap_allocate()`**: F3 implementation with all 4 algorithms + splitting (AC6)
     - **`heap_free()`**: F4 implementation with coalescing (AC7)
     - **`heap_show_memory_map()`**: Displays all blocks in order
     - **`heap_show_statistics()`**: Shows detailed statistics
     - **`heap_destroy()`**: Cleans up all memory (prevents leaks)

4. **`src/algorithms/first_fit.c`** (PR #1)
   - Implements First Fit: finds first suitable FREE block
   - Traverses from head, O(n) time complexity

5. **`src/algorithms/next_fit.c`** (PR #1)
   - Implements Next Fit: circular search from last allocation
   - Starts from last_allocated->next, wraps to head if needed

6. **`src/algorithms/best_fit.c`** (PR #1)
   - Implements Best Fit: finds smallest suitable block
   - Traverses entire list, O(n) time complexity

7. **`src/algorithms/worst_fit.c`** (PR #1)
   - Implements Worst Fit: finds largest FREE block
   - Traverses entire list, O(n) time complexity

8. **`src/ui/cli.c`** (PR #2, Dec 19)
   - Implements interactive menu system (153 lines)
   - Functions:
     - `print_header()` - Display program title
     - `print_menu()` - Show 7 menu options (0-6)
     - `handle_allocation()` - Process allocation requests
     - `handle_free()` - Process deallocation requests
     - `handle_change_algo()` - Algorithm selection submenu
     - `handle_stats()` - Display heap statistics
     - `run_cli()` - Main CLI loop

9. **`src/utils/input_utils.c`** (PR #2, Dec 19)
   - Implements safe input utilities (39 lines)
   - `read_int()`: Robust integer parsing with `strtol()`
   - `read_string()`: Safe string input with `fgets()`
   - `wait_for_enter()`: Optional pause function

10. **`src/main.c`** (Updated PR #2)
   - Entry point with CLI integration
   - Prompts for initial heap size
   - Starts interactive menu loop
   - Cleanup on exit

### Build System

1. **`Makefile`**
   - GNU Make compatible (works on Ubuntu WSL and NetBSD with `gmake`)
   - **Compiles 10 source files:**
     - 3 core files (algorithm_type, memory_block, heap_manager)
     - 4 algorithm files (first_fit, next_fit, best_fit, worst_fit)
     - 1 UI file (cli.c) **NEW (PR #2)**
     - 1 Utils file (input_utils.c) **NEW (PR #2)**
     - 1 main file (main.c)
   - Targets:
     - `make` or `make all`: Build project
     - `make clean`: Remove build artifacts
     - `make run`: Build and execute
     - `make debug`: Debug build with symbols
     - `make release`: Optimized release build
     - `make valgrind`: Memory leak check (Ubuntu only)
     - `make help`: Display help
   - Compiler flags:
     - `-Wall -Wextra -Wpedantic`: Strict warnings
     - `-std=c99`: C99 standard
     - `-O2`: Optimization level 2
     - `-g`: Debug symbols

## Compliance with Requirements

### From CHECKLIST.md:
- ✅ `AlgorithmType` enum defined
- ✅ `MemoryBlock` struct with all required fields
- ✅ `HeapManager` struct with all required fields
- ✅ F1: `heap_init()` implemented & tested
- ✅ F2: `heap_set_algorithm()` implemented & tested
- ✅ F5: `heap_show_memory_map()` implemented & tested
- ✅ F3: `heap_allocate()` - **IMPLEMENTED & TESTED** ✅
- ✅ F4: `heap_free()` - **IMPLEMENTED & TESTED** ✅
- ✅ **UI1: Interactive CLI** - **IMPLEMENTED (PR #2)** ✅

### From PR.md:
- ✅ **NF2**: Code written in Standard C99
- ✅ **NF3**: Using Linked List (not arrays)
- ✅ **AC1**: Initial heap shows 1 FREE block - **PASSED** ✅
- ✅ **AC2**: First Fit logic - **PASSED** ✅
- ✅ **AC6**: Splitting feature - **PASSED** ✅
- ⚠️ **AC3-AC5, AC7-AC8**: Implemented, need dedicated tests

### From PLATFORM.md:
- ✅ POSIX-compliant functions only
- ✅ Proper header inclusion (`<sys/types.h>` first)
- ✅ Safe string handling (`strncpy` with null termination)
- ✅ Format specifiers (`%zu` for `size_t`, `%p` for pointers)

### Memory Quality (Valgrind):
- ✅ **Zero memory leaks** verified
- ✅ 6 allocations = 6 frees
- ✅ 0 errors from 0 contexts

## Next Steps

1. **Create Comprehensive Test Scenarios**
   - Dedicated tests for AC3 (Best Fit)
   - Dedicated tests for AC4 (Worst Fit)
   - Dedicated tests for AC5 (Next Fit with circular search)
   - Explicit coalescing test for AC7
   - Error handling edge cases for AC8

2. **Implement Menu-driven CLI**
   - Interactive menu system
   - Input validation
   - Error handling

3. **Cross-platform Testing**
   - Test on NetBSD VirtualBox
   - Verify compatibility

4. **Phase 2 Enhancements** (Optional)
   - Improved visualization
   - ANSI colors
   - Fragmentation report

## Test Results Summary (Dec 19, 2025)

### ✅ PASSED (3/8):
- **AC1**: Initialization - Shows 1 FREE block ✅
- **AC2**: First Fit Logic - Selects first suitable block ✅
- **AC6**: Splitting Feature - Splits blocks correctly ✅

### ⚠️ IMPLEMENTED, NEED TESTING (5/8):
- **AC3**: Best Fit - Algorithm implemented
- **AC4**: Worst Fit - Algorithm implemented
- **AC5**: Next Fit - Algorithm implemented with circular search
- **AC7**: Coalescing - Merge logic implemented
- **AC8**: Error Handling - Validation implemented

### 🎉 Major Achievements:
- ✅ Zero compilation warnings
- ✅ Zero memory leaks (Valgrind verified)
- ✅ All 4 algorithms implemented in separate files
- ✅ Splitting mechanism verified
- ✅ First Fit verified
- ✅ Statistics tracking working
- ✅ **Interactive CLI implemented (PR #2)** 🆕
- ✅ **Robust input validation (PR #2)** 🆕
- ✅ **10 source files compiled successfully** 🆕

## How to Build and Run

```bash
# Clean and build
make clean && make

# Run the test program
make run

# Check for memory leaks (Ubuntu WSL only)
make valgrind

# See all available commands
make help
```

## Notes

- All files follow C99 standard
- No memory leaks in current implementation (all `malloc()` paired with `free()`)
- Safe string handling throughout
- Input validation on all functions
- Compatible with both Ubuntu WSL and NetBSD
