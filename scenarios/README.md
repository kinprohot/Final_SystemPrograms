# Test Scenarios

This directory contains test scenario files for automated testing of the memory allocator.

## File Format

Each scenario file contains a sequence of commands, one per line:

```
# Comments start with #
alloc <process_id> <size>
free <process_id>
switch <algorithm>
show
stats
```

## Available Commands

- `alloc <pid> <size>` - Allocate memory to process
- `free <pid>` - Free memory of process
- `switch <algo>` - Switch algorithm (ff/nf/bf/wf)
- `show` - Show memory map
- `stats` - Show statistics

## Test Scenarios (To be created)

- `test_basic.txt` - Basic allocation and deallocation
- `test_splitting.txt` - Test block splitting (AC6)
- `test_coalescing.txt` - Test block coalescing (AC7)
- `test_fragmentation.txt` - Worst-case fragmentation scenario
- `test_all_algorithms.txt` - Compare all 4 algorithms

## Usage

```bash
./memory_allocator --script scenarios/test_basic.txt
```

Note: Script mode is a Phase 3 feature (optional).
