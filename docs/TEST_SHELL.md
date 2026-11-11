# INTERACTIVE SHELL TEST PLAN
**Date:** December 20, 2025  
**Phase:** 2B - Interactive Command-Line Shell  
**Acceptance Criteria:** AC-SHELL1 to AC-SHELL7

---

## TEST SETUP

### Build Instructions
```bash
cd /mnt/e/LTHT_project
make clean
make
```

### Expected Output
```
Compiling src/core/algorithm_type.c...
Compiling src/core/memory_block.c...
Compiling src/core/heap_manager.c...
Compiling src/algorithms/first_fit.c...
Compiling src/algorithms/next_fit.c...
Compiling src/algorithms/best_fit.c...
Compiling src/algorithms/worst_fit.c...
Compiling src/ui/cli.c...
Compiling src/ui/visualizer.c...
Compiling src/ui/shell.c...
Compiling src/utils/input_utils.c...
Compiling src/utils/shell_parser.c...
Compiling src/main.c...
Linking memory_allocator...
Build successful! Executable: memory_allocator
```

**Expected File Count:** 13 source files (11 from Phase 2 + shell.c + shell_parser.c)

---

## AC-SHELL1: Interactive Shell Launch

### Test Command
```bash
./memory_allocator -i
```

### Expected Output
```
╔═══════════════════════════════════════════════════════════════╗
║       Memory Allocator Interactive Shell v2.0                ║
║       Algorithms: First Fit, Next Fit, Best Fit, Worst Fit   ║
╚═══════════════════════════════════════════════════════════════╝

Type 'help' for commands, 'exit' to quit

allocator> 
```

### Acceptance Criteria
- [ ] Shell launches without errors
- [ ] Welcome banner displays correctly
- [ ] Prompt appears: `allocator>`
- [ ] Waits for user input

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## AC-SHELL2: Stateful Session

### Test Commands
```bash
./memory_allocator -i

allocator> init 1024
allocator> alloc P1:100
allocator> alloc P2:200
allocator> show
allocator> stat
allocator> exit
```

### Expected Output
```
allocator> init 1024
✓ Heap initialized: 1024 bytes

allocator> alloc P1:100
✓ Allocated 100 bytes for P1 at address 0

allocator> alloc P2:200
✓ Allocated 200 bytes for P2 at address 100

allocator> show

═══════════════════════════════════════════════════════════════
                        MEMORY LAYOUT
═══════════════════════════════════════════════════════════════
Scale: 0     102    204    ...    1024
       ┌────────────────────────────────────┐
       │[P1:100][P2:200][FREE:724]          │
       └────────────────────────────────────┘

Block Details:
       Block 1: [Addr: 0    | Size: 100  | ALLOCATED: P1]
       Block 2: [Addr: 100  | Size: 200  | ALLOCATED: P2]
       Block 3: [Addr: 300  | Size: 724  | FREE]
═══════════════════════════════════════════════════════════════

allocator> stat

╔═══════════════════════════════════════════════════════════════╗
║  Memory: 300/1024 (29%)  │  Algorithm: First Fit            ║
║  Allocs: 2    │  Deallocs: 0    │  Fails: 0    │  Free: 1  ║
╚═══════════════════════════════════════════════════════════════╝

allocator> exit
Goodbye!
```

### Acceptance Criteria
- [ ] Heap persists across multiple commands
- [ ] Allocations accumulate correctly
- [ ] Memory map shows both allocations
- [ ] Statistics reflect correct state
- [ ] Exit command terminates gracefully

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## AC-SHELL3: Algorithm Selection

### Test Commands
```bash
./memory_allocator -i

allocator> init 500
allocator> alloc A:100
allocator> alloc B:100
allocator> alloc C:100
allocator> free B
allocator> algo bf
allocator> alloc D:50
allocator> show
allocator> exit
```

### Expected Behavior
1. Creates state: `[A:100][FREE:100][C:100][FREE:200]`
2. Sets algorithm to Best Fit
3. Allocates D:50 into 100-byte hole (best fit)
4. Memory map shows: `[A:100][D:50][FREE:50][C:100][FREE:200]`

### Expected Output (partial)
```
allocator> algo bf
✓ Algorithm set to: Best Fit

allocator> alloc D:50
✓ Allocated 50 bytes for D at address 100

allocator> show
       Block 1: [Addr: 0    | Size: 100  | ALLOCATED: A]
       Block 2: [Addr: 100  | Size: 50   | ALLOCATED: D]
       Block 3: [Addr: 150  | Size: 50   | FREE]
       Block 4: [Addr: 200  | Size: 100  | ALLOCATED: C]
       Block 5: [Addr: 300  | Size: 200  | FREE]
```

### Acceptance Criteria
- [ ] `algo bf` command sets Best Fit
- [ ] Best Fit selects 100-byte hole (not 200-byte)
- [ ] Block splits correctly (50 allocated, 50 remains free)
- [ ] Memory map reflects correct allocation

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## AC-SHELL4: Error Handling

### Test Commands
```bash
./memory_allocator -i

allocator> alloc P1:100
allocator> init 500
allocator> alloc P1:1000
allocator> alloc P2:100
allocator> free P99
allocator> algo invalid
allocator> alloc
allocator> free
allocator> exit
```

### Expected Output
```
allocator> alloc P1:100
✗ Error: Heap not initialized. Use 'init <size>' first

allocator> init 500
✓ Heap initialized: 500 bytes

allocator> alloc P1:1000
✗ Error: Not enough memory (requested: 1000, available: 500)

allocator> alloc P2:100
✓ Allocated 100 bytes for P2 at address 0

allocator> free P99
✗ Error: Process 'P99' not found

allocator> algo invalid
✗ Error: Invalid algorithm 'invalid'. Use: ff, nf, bf, wf

allocator> alloc
✗ Error: Missing size. Usage: alloc <PID>:<size>

allocator> free
✗ Error: Missing process ID. Usage: free <PID>

allocator> exit
Goodbye!
```

### Acceptance Criteria
- [ ] Commands before init show error
- [ ] Oversized allocation shows error
- [ ] Non-existent PID shows error
- [ ] Invalid algorithm shows error
- [ ] Missing arguments show usage hints
- [ ] Program doesn't crash on any error

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## AC-SHELL5: Batch Mode

### Test File: `scenarios/test_shell.txt`
```
init 1000
algo bf
alloc P1:100
alloc P2:300
alloc P3:200
show
free P2
alloc P4:50
show
stat
frag
reset
show
```

### Test Command
```bash
./memory_allocator -b scenarios/test_shell.txt
```

### Expected Output (partial)
```
Executing batch file: scenarios/test_shell.txt

[Line 1] init 1000
✓ Heap initialized: 1000 bytes

[Line 2] algo bf
✓ Algorithm set to: Best Fit

[Line 3] alloc P1:100
✓ Allocated 100 bytes for P1 at address 0

[Line 4] alloc P2:300
✓ Allocated 300 bytes for P2 at address 100

[Line 5] alloc P3:200
✓ Allocated 200 bytes for P3 at address 400

[Line 6] show
[Memory map displayed]

[Line 7] free P2
✓ P2 freed

[Line 8] alloc P4:50
✓ Allocated 50 bytes for P4 at address 100

[Line 9] show
[Memory map displayed]

[Line 10] stat
[Statistics displayed]

[Line 11] frag
[Fragmentation report displayed]

[Line 12] reset
✓ Heap reset to initial state (1000 bytes)

[Line 13] show
[Empty heap displayed]

Batch execution complete: 0 errors
```

### Acceptance Criteria
- [ ] Batch file executes line by line
- [ ] Each command echoed before execution
- [ ] Output shows results of each command
- [ ] No errors reported
- [ ] Final summary shows 0 errors

### Stdin Redirection Test
```bash
./memory_allocator < scenarios/test_shell.txt
```

### Acceptance Criteria
- [ ] Stdin redirection works
- [ ] Same behavior as -b flag

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## AC-SHELL6: Backward Compatibility

### Test Command
```bash
./memory_allocator
```

### Expected Output (Menu Mode)
```
=== Memory Allocation Simulator ===
Enter initial heap size (bytes) [Default 1024]: 1024
Initializing heap with 1024 bytes...

╔═══════════════════════════════════════════════════════════════╗
║  Memory: 0/1024 (0%)  │  Algorithm: First Fit               ║
║  Allocs: 0    │  Deallocs: 0    │  Fails: 0    │  Free: 1  ║
╚═══════════════════════════════════════════════════════════════╝

=============================================
      MEMORY ALLOCATION SIMULATOR
=============================================

MAIN MENU:
1. Allocate Memory
2. Free Memory
3. Show Memory Map
4. Change Algorithm
5. Display Statistics
6. Display Fragmentation Report
7. Toggle Color Mode
0. Exit

Enter your choice:
```

### Acceptance Criteria
- [ ] Default mode (no args) launches menu
- [ ] Menu mode works as before (Phase 1)
- [ ] All menu options functional
- [ ] No breaking changes

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## AC-SHELL7: Help System

### Test Commands
```bash
./memory_allocator --help
./memory_allocator -h
```

### Expected Output
```
Memory Allocator v2.0 - Memory Allocation Algorithms Simulator
===============================================================

Usage:
  ./memory_allocator              Interactive menu mode (default)
  ./memory_allocator -i           Interactive shell mode (REPL)
  ./memory_allocator -b <file>    Batch mode (execute script file)
  ./memory_allocator --help       Display this help message
  ./memory_allocator --version    Display version information

Interactive Shell Commands:
  init <size>           Initialize heap with size (bytes)
  alloc <PID>:<size>    Allocate memory for process
  free <PID>            Free memory of process
  show                  Display memory map
  stat                  Display heap statistics
  frag                  Display fragmentation report
  algo <ff|nf|bf|wf>    Set allocation algorithm
  reset                 Reset heap to initial state
  help                  Display help message
  exit                  Exit shell

Algorithms:
  ff   First Fit   - Find first free block large enough
  nf   Next Fit    - Start from last allocated position
  bf   Best Fit    - Find smallest sufficient free block
  wf   Worst Fit   - Find largest free block

Examples:
  ./memory_allocator -i
  allocator> init 1024
  allocator> alloc P1:100
  allocator> show

For more information, see docs/RECIPE_CMD.md
```

### Shell Help Command
```bash
./memory_allocator -i

allocator> help
```

### Expected Output
```
Available Commands:
  init <size>            Initialize heap with size (bytes)
  alloc <PID>:<size>     Allocate memory for process
  free <PID>             Free memory of process
  show                   Display memory map
  stat                   Display heap statistics
  frag                   Display fragmentation report
  algo <ff|nf|bf|wf>     Set allocation algorithm
                         (ff=First Fit, nf=Next Fit,
                          bf=Best Fit, wf=Worst Fit)
  reset                  Reset heap to initial state
  help                   Display this help message
  exit                   Exit shell

Examples:
  init 1024
  alloc P1:100
  alloc P2:200
  show
  free P1
  algo bf
```

### Version Command
```bash
./memory_allocator --version
```

### Expected Output
```
Memory Allocator v2.0 (Phase 2B)
Algorithms: First Fit, Next Fit, Best Fit, Worst Fit
Features: Interactive Shell, Batch Mode, Enhanced Visualization
```

### Acceptance Criteria
- [ ] `--help` displays full help message
- [ ] `-h` also works
- [ ] `help` command in shell displays commands
- [ ] `--version` displays version info
- [ ] `-v` also works
- [ ] Examples section included

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## ADDITIONAL TESTS

### Test 8: Command Aliases
```bash
./memory_allocator -i

allocator> init 1000
allocator> a P1:100     # alias for alloc
allocator> s             # alias for show
allocator> st            # alias for stat
allocator> f P1          # alias for free
allocator> q             # alias for quit
```

### Expected Behavior
- `a` → `alloc`
- `s` → `show`
- `st` → `stat`
- `f` or `fr` → `free` or `frag` (context-dependent)
- `q` → `quit`

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

### Test 9: Comments in Batch Files
```bash
# Create test file with comments
cat > scenarios/test_comments.txt << EOF
# This is a comment
init 1000
# Another comment
alloc P1:100
# Test allocation
show
EOF

./memory_allocator -b scenarios/test_comments.txt
```

### Expected Behavior
- Comment lines ignored
- Commands execute normally

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

### Test 10: Empty Lines
```bash
./memory_allocator -i

allocator> init 1000
allocator> 
allocator> alloc P1:100
allocator> 
allocator> 
allocator> show
```

### Expected Behavior
- Empty lines (just Enter) ignored
- No error messages
- Prompt reappears

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## MEMORY LEAK CHECK

### Valgrind Test (Ubuntu/WSL Only)
```bash
# Interactive mode (with EOF)
echo -e "init 1000\nalloc P1:100\nexit" | valgrind --leak-check=full ./memory_allocator -i

# Batch mode
valgrind --leak-check=full ./memory_allocator -b scenarios/test_shell.txt
```

### Expected Output
```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
  total heap usage: X allocs, X frees, Y bytes allocated

All heap blocks were freed -- no leaks are possible
```

### Status: ⬜ PENDING / ✅ PASSED / ❌ FAILED

---

## SUMMARY

### Phase 2B Completion Checklist

- [ ] AC-SHELL1: Interactive Shell Launch
- [ ] AC-SHELL2: Stateful Session
- [ ] AC-SHELL3: Algorithm Selection
- [ ] AC-SHELL4: Error Handling
- [ ] AC-SHELL5: Batch Mode
- [ ] AC-SHELL6: Backward Compatibility
- [ ] AC-SHELL7: Help System
- [ ] Test 8: Command Aliases
- [ ] Test 9: Comments in Batch Files
- [ ] Test 10: Empty Lines
- [ ] Memory Leak Check (Valgrind)

### Files Created
- [x] `src/utils/shell_parser.h`
- [x] `src/utils/shell_parser.c`
- [x] `src/ui/shell.h`
- [x] `src/ui/shell.c`
- [x] `scenarios/test_shell.txt`
- [x] Updated `src/main.c`
- [x] Updated `Makefile`

### Build Status
- [ ] Zero compilation warnings
- [ ] Zero compilation errors
- [ ] 13 source files compiled

### Overall Status
- **Total Tests:** 11
- **Passed:** 0 / 11
- **Failed:** 0 / 11
- **Pending:** 11 / 11

---

## NEXT STEPS

1. **Build Project:**
   ```bash
   make clean && make
   ```

2. **Quick Test:**
   ```bash
   ./memory_allocator -i
   ```

3. **Run Acceptance Tests:**
   - AC-SHELL1 to AC-SHELL7
   - Additional Tests 8-10

4. **Memory Leak Check:**
   ```bash
   valgrind --leak-check=full ./memory_allocator -b scenarios/test_shell.txt
   ```

5. **Update Documentation:**
   - Mark completed tests in CHECKLIST.md
   - Update PR.md with Phase 2B completion
   - Create demo screenshots/examples

---

**Test Plan Version:** 1.0  
**Date:** December 20, 2025  
**Prepared By:** AI Assistant  
**Status:** Ready for Testing
