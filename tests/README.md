# Test Suite

This directory will contain test files for unit testing and integration testing.

## Planned Test Files

- `test_allocator.c` - Unit tests for allocation algorithms
- `test_heap_manager.c` - Tests for heap management functions
- `test_splitting.c` - Tests for block splitting mechanism
- `test_coalescing.c` - Tests for block coalescing mechanism
- `test_scenarios.c` - Integration tests with various scenarios
- `benchmark.c` - Performance benchmarking

## Running Tests

```bash
# Compile tests
make tests

# Run all tests
./run_tests

# Run with Valgrind
valgrind --leak-check=full ./run_tests
```

Note: Test framework to be implemented in later phases.
