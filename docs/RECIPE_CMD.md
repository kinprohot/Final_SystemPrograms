# COMMAND-LINE INTERFACE RECIPE
**Project:** Memory Allocation Algorithms Simulator  
**Phase:** Phase 2B Enhancement  
**Feature:** Interactive Command-Line Shell (REPL)  
**Date:** December 20, 2025

---

## 📋 OVERVIEW

Transform `memory_allocator` into a professional command-line tool supporting **3 distinct modes**:

### **MODE 1: Interactive Menu** (Phase 1 - Existing)
- Traditional numbered menu with loop
- User-friendly for beginners
- Stateful session with heap persistence

### **MODE 2: Interactive Shell** (Phase 2B - NEW ⭐)
- REPL (Read-Eval-Print Loop) with command prompt
- Type commands directly: `alloc P1:100`, `show`, `stat`
- Stateful session - heap persists across commands
- Demo-friendly: See results step-by-step
- Professional: Similar to `gdb`, `psql`, `redis-cli`

### **MODE 3: Batch/Script** (Phase 3 - Future)
- Execute commands from file
- Fully automated testing
- CI/CD integration

---

## 🎯 THE STATEFULNESS PROBLEM & SOLUTION

### ❌ **Why One-Line Flags DON'T WORK:**

```bash
# Problem: Each invocation = New process = No state!
./memory_allocator --init 1024
# ✅ Heap created → ❌ Program exits → 💀 Heap destroyed

./memory_allocator --show
# ❌ ERROR: No heap exists! (new process)

# "Solution": One-line hell
./memory_allocator --init 1024 --alloc P1:100 --alloc P2:200 --free P1 --show --stat --frag
# ❌ Too long, can't see intermediate results, bad for demo
```

### ✅ **Interactive Shell SOLVES THIS:**

```bash
./memory_allocator -i
# Enter interactive shell - ONE PROCESS, PERSISTENT STATE

allocator> init 1024
✓ Heap initialized: 1024 bytes

allocator> alloc P1:100
✓ Allocated 100 bytes for P1

allocator> show
[Memory map displayed - heap still exists!]

allocator> alloc P2:200
✓ Allocated 200 bytes for P2

allocator> exit
Goodbye!
```

**Benefits:**
- ✅ **Stateful**: Heap exists throughout session
- ✅ **Step-by-step**: See each operation's result
- ✅ **Demo-friendly**: Type → Enter → See result
- ✅ **Scriptable**: Can pipe commands or redirect stdin
- ✅ **Professional**: Industry-standard REPL pattern

---

## 🎬 DESIGN PHILOSOPHY

```
┌─────────────────────────────────────────────────────┐
│  ./memory_allocator                                 │
│  → MODE 1: Interactive Menu (Phase 1)              │
│  → Numbered options, loop, beginner-friendly       │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│  ./memory_allocator -i  (or --interactive)         │
│  → MODE 2: Interactive Shell (Phase 2B) ⭐         │
│  → Command prompt, REPL, demo-friendly             │
│  → Stateful, professional, scriptable              │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│  ./memory_allocator -b script.txt                   │
│  ./memory_allocator < script.txt                    │
│  → MODE 3: Batch Mode (Phase 3)                    │
│  → Automated, CI/CD, reproducible                  │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│  ./memory_allocator --help                          │
│  ./memory_allocator --version                       │
│  → Quick info, exit immediately                     │
└─────────────────────────────────────────────────────┘
```

---

## 🎯 COMMAND-LINE SYNTAX

```bash
# Mode 1: Interactive Menu (default - backward compatible)
./memory_allocator

# Mode 2: Interactive Shell (NEW)
./memory_allocator -i
./memory_allocator --interactive

# Mode 3: Batch/Script (Phase 3)
./memory_allocator -b script.txt
./memory_allocator --batch script.txt
./memory_allocator < script.txt

# Quick commands
./memory_allocator --help
./memory_allocator --version
```

---

## 📖 SHELL COMMAND REFERENCE

### **Core Commands**

| Command | Syntax | Description | Example |
|---------|--------|-------------|---------|
| `init` | `init <size>` | Initialize heap with size (bytes) | `init 1024` |
| `alloc` | `alloc <PID>:<size>` | Allocate memory for process | `alloc P1:100` |
| `free` | `free <PID>` | Free memory of process | `free P1` |
| `show` | `show [mode]` | Display memory map | `show`, `show detail`, `show simple` |
| `stat` | `stat` | Display heap statistics | `stat` |
| `frag` | `frag` | Display fragmentation report | `frag` |
| `algo` | `algo <ff\|nf\|bf\|wf>` | Set allocation algorithm | `algo bf` |
| `reset` | `reset` | Reset heap to initial state | `reset` |
| `help` | `help` | Display command list | `help` |
| `exit` | `exit` or `quit` | Exit shell | `exit` |

### **Show Command Modes**
- `show` - Beautiful visualization with memory bar (default)
- `show simple` - Text-only view (old style)
- `show detail` - Full analysis (stats + visualization + fragmentation)

### **Algorithm Codes**
- `ff` = First Fit
- `nf` = Next Fit
- `bf` = Best Fit
- `wf` = Worst Fit

### **Command Aliases (Optional)**
```
a     → alloc
f     → free
s     → show
st    → stat
fr    → frag
q     → quit
?     → help
```

---

## 💡 USAGE EXAMPLES

### Example 1: Interactive Shell Session (Demo for Instructor)
```bash
$ ./memory_allocator -i

Memory Allocator Interactive Shell v2.0
Type 'help' for commands, 'exit' to quit

allocator> init 1000
✓ Heap initialized: 1000 bytes

allocator> algo bf
✓ Algorithm set to: Best Fit

allocator> alloc P1:100
✓ Allocated 100 bytes for P1 at address 0

allocator> alloc P2:300
✓ Allocated 300 bytes for P2 at address 100

allocator> alloc P3:200
✓ Allocated 200 bytes for P3 at address 400

allocator> show

═══════════════════════════════════════════════════════════════════════════
                        MEMORY LAYOUT
═══════════════════════════════════════════════════════════════════════════
Scale: 0     100     200     300     400     500     600     700     800     900     1000
       ┌────────────────────────────────────────────────────────────┐
       │██████████████████████████████████████░░░░░░░░░░░░░░░░░░░░░░│
       └────────────────────────────────────────────────────────────┘
       Legend:  █ Allocated    ░ Free

       Block Details:
       ───────────────────────────────────────────────────────
       Block 1: [Addr: 0    | Size: 100  | PID: P1]
       Block 2: [Addr: 100  | Size: 300  | PID: P2]
       Block 3: [Addr: 400  | Size: 200  | PID: P3]
       Block 4: [Addr: 600  | Size: 400  | FREE]
═══════════════════════════════════════════════════════════════════════════

allocator> free P2
✓ P2 freed (300 bytes)

allocator> alloc P4:50
✓ Allocated 50 bytes for P4 at address 100
   (Best Fit selected 300-byte hole)

allocator> frag

╔═══════════════════════════════════════════════════════════════════════╗
║              FRAGMENTATION ANALYSIS                                    ║
╠═══════════════════════════════════════════════════════════════════════╣
║  Total Free Space:        650    bytes                                 ║
║  Largest Free Block:      400    bytes                                 ║
║  Number of Free Blocks:   2                                          ║
║  External Fragmentation:  38.5% (Moderate)                             ║
╚═══════════════════════════════════════════════════════════════════════╝

allocator> stat

╔═══════════════════════════════════════════════════════════════════════╗
║  Memory: 350/1000 (35%)  │  Algorithm: Best Fit                  ║
║  Allocs: 4    │  Deallocs: 1    │  Fails: 0    │  Free Blocks: 2    ║
╚═══════════════════════════════════════════════════════════════════════╝

allocator> exit
Goodbye!
```

### Example 2: Testing Algorithm Behavior
```bash
$ ./memory_allocator -i

allocator> init 500
✓ Heap initialized: 500 bytes

allocator> alloc A:100
✓ Allocated 100 bytes for A at address 0

allocator> alloc B:100
✓ Allocated 100 bytes for B at address 100

allocator> alloc C:100
✓ Allocated 100 bytes for C at address 200

allocator> free B
✓ B freed (100 bytes)

# Now we have: [A:100] [FREE:100] [C:100] [FREE:200]

allocator> algo ff
✓ Algorithm set to: First Fit

allocator> alloc D:50
✓ Allocated 50 bytes for D at address 100
   (First Fit chose first hole)

allocator> reset
✓ Heap reset to initial state

allocator> alloc A:100
allocator> alloc B:100
allocator> alloc C:100
allocator> free B

allocator> algo bf
✓ Algorithm set to: Best Fit

allocator> alloc D:50
✓ Allocated 50 bytes for D at address 100
   (Best Fit chose 100-byte hole, exact fit)

allocator> exit
```

### Example 3: Fragmentation Analysis
```bash
$ ./memory_allocator -i

allocator> init 1000
allocator> alloc P1:100
allocator> alloc P2:200
allocator> alloc P3:100
allocator> alloc P4:200
allocator> alloc P5:100

allocator> show
# [P1:100][P2:200][P3:100][P4:200][P5:100][FREE:300]

allocator> free P2
allocator> free P4

allocator> show
# [P1:100][FREE:200][P3:100][FREE:200][P5:100][FREE:300]

allocator> frag
╔═══════════════════════════════════════════════════════════════════════╗
║  Total Free Space:        700    bytes                                 ║
║  Largest Free Block:      300    bytes                                 ║
║  Number of Free Blocks:   3                                          ║
║  External Fragmentation:  57.1% (Moderate)                             ║
╚═══════════════════════════════════════════════════════════════════════╝

allocator> exit
```

### Example 4: Batch Mode (Phase 3 - Future)
```bash
# Create test_scenario.txt
$ cat > test_scenario.txt << EOF
init 1000
algo bf
alloc Process_A:150
alloc Process_B:250
alloc Process_C:100
show
free Process_B
alloc Process_D:100
show
stat
frag
EOF

# Execute batch
$ ./memory_allocator -b test_scenario.txt

✓ Heap initialized: 1000 bytes
✓ Algorithm set to: Best Fit
✓ Allocated 150 bytes for Process_A
✓ Allocated 250 bytes for Process_B
✓ Allocated 100 bytes for Process_C

[Memory map displayed]

✓ Process_B freed
✓ Allocated 100 bytes for Process_D

[Memory map displayed]
[Statistics displayed]
[Fragmentation report displayed]

# Or use stdin redirection
$ ./memory_allocator < test_scenario.txt
```

### Example 5: Error Handling
```bash
$ ./memory_allocator -i

allocator> init 500
✓ Heap initialized: 500 bytes

allocator> alloc P1:1000
✗ Error: Not enough memory (requested: 1000, available: 500)

allocator> alloc P2:100
✓ Allocated 100 bytes for P2

allocator> free P99
✗ Error: Process 'P99' not found

allocator> algo invalid
✗ Error: Invalid algorithm. Use: ff, nf, bf, wf

allocator> alloc
✗ Error: Invalid syntax. Usage: alloc <PID>:<size>

allocator> exit
```

### Example 6: Quick Help
```bash
$ ./memory_allocator --help

Memory Allocator v2.0 - Memory Allocation Algorithms Simulator
===============================================================

Usage:
  ./memory_allocator              Interactive menu mode (default)
  ./memory_allocator -i           Interactive shell mode
  ./memory_allocator -b <file>    Batch mode (execute commands from file)
  ./memory_allocator --help       Display this help
  ./memory_allocator --version    Display version information

Interactive Shell Commands:
  init <size>           Initialize heap with size (bytes)
  alloc <PID>:<size>    Allocate memory for process
  free <PID>            Free memory of process
  algo <ff|nf|bf|wf>    Set allocation algorithm
  show                  Display memory map
  stat                  Display statistics
  frag                  Display fragmentation report
  reset                 Reset heap to initial state
  help                  Display command list
  exit                  Exit shell

Algorithms:
  ff   First Fit   - Find first free block large enough
  nf   Next Fit    - Find next free block from last position
  bf   Best Fit    - Find smallest free block that fits
  wf   Worst Fit   - Find largest free block

Examples:
  ./memory_allocator -i
  allocator> init 1024
  allocator> alloc P1:100
  allocator> show

For more information, see docs/RECIPE_CMD.md
```

---

## 🔧 IMPLEMENTATION DETAILS

### File Structure
```
src/
  utils/
    shell_parser.c        # NEW: Command parser for shell mode
    shell_parser.h        # NEW: Command parsing declarations
  ui/
    shell.c               # NEW: Interactive shell (REPL)
    shell.h               # NEW: Shell interface
    cli.c                 # EXISTING: Menu mode (unchanged)
    cli.h                 # EXISTING: Menu interface
  main.c                  # MODIFY: Mode selection logic
```

### Architecture: REPL (Read-Eval-Print Loop)

```c
// src/ui/shell.c - Main REPL loop
void run_interactive_shell(void) {
    HeapManager* heap = NULL;
    char command[256];
    int running = 1;
    
    /* Initialize color support */
    g_use_colors = supports_color();
    
    printf("Memory Allocator Interactive Shell v2.0\n");
    printf("Type 'help' for commands, 'exit' to quit\n\n");
    
    while (running) {
        printf("allocator> ");
        fflush(stdout);
        
        /* Read command */
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;  /* EOF (Ctrl+D) */
        }
        
        /* Remove trailing newline */
        command[strcspn(command, "\n")] = 0;
        
        /* Skip empty lines */
        if (strlen(command) == 0) {
            continue;
        }
        
        /* Parse and execute */
        int result = execute_shell_command(command, &heap, &running);
        
        if (result != 0) {
            /* Error handled inside execute_shell_command */
        }
    }
    
    /* Cleanup */
    if (heap) {
        heap_destroy(heap);
    }
    printf("Goodbye!\n");
}
```

### Command Parser

```c
// src/utils/shell_parser.c
typedef enum {
    CMD_INIT,
    CMD_ALLOC,
    CMD_FREE,
    CMD_ALGO,
    CMD_SHOW,
    CMD_STAT,
    CMD_FRAG,
    CMD_RESET,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

typedef struct {
    CommandType type;
    char arg1[64];  /* PID or algorithm */
    size_t arg2;    /* Size */
} ParsedCommand;

/* Parse command string */
ParsedCommand parse_shell_command(const char* input);

/* Execute parsed command */
int execute_shell_command(const char* input, HeapManager** heap, int* running);
```

### Command Handler Examples

```c
/* Handle 'init <size>' command */
int handle_init_command(HeapManager** heap, size_t size) {
    if (*heap) {
        heap_destroy(*heap);
    }
    
    *heap = heap_init(size);
    if (*heap) {
        printf("✓ Heap initialized: %zu bytes\n", size);
        return 0;
    } else {
        printf("✗ Error: Failed to initialize heap\n");
        return 1;
    }
}

/* Handle 'alloc <PID>:<size>' command */
int handle_alloc_command(HeapManager* heap, const char* arg) {
    if (!heap) {
        printf("✗ Error: Heap not initialized. Use 'init <size>' first.\n");
        return 1;
    }
    
    /* Parse PID:size format */
    char pid[32];
    size_t size;
    if (sscanf(arg, "%31[^:]:%zu", pid, &size) != 2) {
        printf("✗ Error: Invalid syntax. Usage: alloc <PID>:<size>\n");
        return 1;
    }
    
    MemoryBlock* block = heap_allocate(heap, pid, size);
    if (block) {
        printf("✓ Allocated %zu bytes for %s at address %zu\n", 
               size, pid, block->start_address);
        return 0;
    } else {
        printf("✗ Error: Not enough memory (requested: %zu)\n", size);
        return 1;
    }
}

/* Handle 'free <PID>' command */
int handle_free_command(HeapManager* heap, const char* pid) {
    if (!heap) {
        printf("✗ Error: Heap not initialized\n");
        return 1;
    }
    
    int result = heap_free(heap, pid);
    if (result == 1) {
        printf("✓ %s freed\n", pid);
        return 0;
    } else {
        printf("✗ Error: Process '%s' not found\n", pid);
        return 1;
    }
}

/* Handle 'algo <ff|nf|bf|wf>' command */
int handle_algo_command(HeapManager* heap, const char* algo) {
    if (!heap) {
        printf("✗ Error: Heap not initialized\n");
        return 1;
    }
    
    AlgorithmType type;
    if (strcmp(algo, "ff") == 0) {
        type = FIRST_FIT;
    } else if (strcmp(algo, "nf") == 0) {
        type = NEXT_FIT;
    } else if (strcmp(algo, "bf") == 0) {
        type = BEST_FIT;
    } else if (strcmp(algo, "wf") == 0) {
        type = WORST_FIT;
    } else {
        printf("✗ Error: Invalid algorithm. Use: ff, nf, bf, wf\n");
        return 1;
    }
    
    heap_set_algorithm(heap, type);
    printf("✓ Algorithm set to: %s\n", get_algo_name(type));
    return 0;
}
```

### Main Entry Point

```c
// src/main.c
int main(int argc, char* argv[]) {
    /* Parse command-line arguments */
    if (argc == 1) {
        /* Mode 1: Interactive Menu (default) */
        HeapManager* heap = heap_init(1024);
        run_cli(heap);
        heap_destroy(heap);
        return 0;
    }
    
    if (argc == 2) {
        if (strcmp(argv[1], "-i") == 0 || 
            strcmp(argv[1], "--interactive") == 0) {
            /* Mode 2: Interactive Shell */
            run_interactive_shell();
            return 0;
        }
        
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            print_help();
            return 0;
        }
        
        if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
            print_version();
            return 0;
        }
    }
    
    if (argc == 3) {
        if (strcmp(argv[1], "-b") == 0 || 
            strcmp(argv[1], "--batch") == 0) {
            /* Mode 3: Batch file */
            return run_batch_mode(argv[2]);
        }
    }
    
    /* Invalid arguments */
    printf("Usage: %s [-i|--interactive] [-b|--batch <file>] [--help] [--version]\n", argv[0]);
    return 1;
}
```

---

## 🧪 TESTING SCENARIOS

### Test 1: Interactive Shell - Basic Operations
```bash
$ ./memory_allocator -i

allocator> init 1024
✓ Heap initialized: 1024 bytes

allocator> alloc P1:100
✓ Allocated 100 bytes for P1

allocator> show
[Memory map should show 1 allocated block]

allocator> stat
[Statistics should show 1 allocation, 0 deallocations]

allocator> exit
```

**Expected:** All commands execute successfully, heap state persists throughout session.

### Test 2: Algorithm Switching
```bash
$ ./memory_allocator -i

allocator> init 1000
allocator> alloc P1:300
allocator> alloc P2:400
allocator> free P1

# Now: [FREE:300] [P2:400] [FREE:300]

allocator> algo ff
✓ Algorithm set to: First Fit

allocator> alloc P3:100
# Expected: Allocated in first hole (address 0)

allocator> reset
allocator> alloc P1:300
allocator> alloc P2:400
allocator> free P1

allocator> algo bf
✓ Algorithm set to: Best Fit

allocator> alloc P3:100
# Expected: Allocated in first hole (300 bytes, exact fit)

allocator> exit
```

**Expected:** Different algorithms produce different allocation patterns.

### Test 3: Error Handling
```bash
$ ./memory_allocator -i

allocator> alloc P1:100
✗ Error: Heap not initialized

allocator> init 500
✓ Heap initialized: 500 bytes

allocator> alloc P1:1000
✗ Error: Not enough memory

allocator> alloc P2:100
✓ Allocated 100 bytes for P2

allocator> free P99
✗ Error: Process 'P99' not found

allocator> algo invalid
✗ Error: Invalid algorithm

allocator> free
✗ Error: Invalid syntax

allocator> exit
```

**Expected:** All errors caught gracefully, program doesn't crash.

### Test 4: Batch Mode (stdin redirection)
```bash
$ cat > test.txt << EOF
init 1000
alloc P1:100
alloc P2:200
show
free P1
alloc P3:50
show
stat
EOF

$ ./memory_allocator < test.txt

✓ Heap initialized: 1000 bytes
✓ Allocated 100 bytes for P1
✓ Allocated 200 bytes for P2
[Memory map 1]
✓ P1 freed
✓ Allocated 50 bytes for P3
[Memory map 2]
[Statistics]
```

**Expected:** Commands execute sequentially, output displayed for each step.

### Test 5: Batch Mode (file argument)
```bash
$ ./memory_allocator -b test.txt

[Same output as Test 4]
```

**Expected:** Same behavior as stdin redirection.

### Test 6: Help & Version
```bash
$ ./memory_allocator --help
[Help message displayed]

$ ./memory_allocator --version
Memory Allocator v2.0 (Phase 2B)
Algorithms: First Fit, Next Fit, Best Fit, Worst Fit
```

**Expected:** Quick info displayed, program exits immediately.

### Test 7: Backward Compatibility
```bash
$ ./memory_allocator

=============================================
      MEMORY ALLOCATION SIMULATOR            
=============================================

MAIN MENU:
1. Allocate Memory
2. Free Memory
...
```

**Expected:** Traditional menu mode works as before (Phase 1 behavior).

### Test 8: Fragmentation Analysis
```bash
$ ./memory_allocator -i

allocator> init 1000
allocator> alloc P1:100
allocator> alloc P2:100
allocator> alloc P3:100
allocator> alloc P4:100
allocator> free P2
allocator> free P4

allocator> frag
╔═══════════════════════════════════════════════════════════════════════╗
║  External Fragmentation:  50.0% (Moderate)                             ║
╚═══════════════════════════════════════════════════════════════════════╝

allocator> exit
```

**Expected:** Fragmentation calculation correct, color-coded severity displayed.

---

## 📊 COMPARISON: 3 MODES

| Feature | Menu Mode | Interactive Shell | Batch Mode |
|---------|-----------|-------------------|------------|
| **Launch** | `./memory_allocator` | `./memory_allocator -i` | `./memory_allocator -b file.txt` |
| **Stateful** | ✅ Yes | ✅ Yes | ✅ Yes (within script) |
| **Step-by-step** | ✅ Numbered menu | ✅ Command prompt | ❌ Automatic |
| **See intermediate** | ✅ Yes | ✅ Yes | ✅ Yes (can see each step) |
| **Demo-friendly** | ✅ Excellent | ✅ Excellent | ⚠️ OK (shows output) |
| **Learning curve** | Easy | Easy | Easy |
| **Script-able** | ❌ No | ✅ Yes (stdin pipe) | ✅ Yes (file) |
| **Automation** | ❌ Manual | ⚠️ Limited | ✅ Full |
| **Professional** | ⚠️ Student-like | ✅ Production-like | ✅ Production |
| **Use case** | Learning, demo | Demo, testing | CI/CD, regression |

---

## 🎯 BENEFITS

### Mode 1: Interactive Menu
**Best for:**
- ✅ First-time users
- ✅ Learning the tool
- ✅ Step-by-step exploration
- ✅ No command syntax to remember

**Limitations:**
- ❌ Slow for repeated operations
- ❌ Not scriptable
- ❌ Hard to share workflows

### Mode 2: Interactive Shell ⭐ **RECOMMENDED FOR DEMO**
**Best for:**
- ✅ **Instructor demonstrations** - Type → Enter → See result
- ✅ **Algorithm comparisons** - Quick reset, try different algo
- ✅ **Debugging sessions** - Inspect state after each operation
- ✅ **Professional presentations** - Looks like production tool

**Key advantages:**
- ✅ **Stateful** - Heap persists across commands
- ✅ **Visual** - See results immediately
- ✅ **Flexible** - Easy to backtrack (reset) and try different paths
- ✅ **Scriptable** - Can pipe commands: `echo "init 1024\nalloc P1:100\nshow" | ./memory_allocator -i`

### Mode 3: Batch Mode
**Best for:**
- ✅ Automated testing
- ✅ Reproducible benchmarks
- ✅ CI/CD integration
- ✅ Regression testing

**Advantages:**
- ✅ Fully automated
- ✅ Shareable test cases
- ✅ Version control (scripts)

---

## 🚀 IMPLEMENTATION PRIORITY

### Phase 2B.1: Core Shell (2-3 hours)
- [ ] Create `src/ui/shell.c` with REPL loop
- [ ] Create `src/utils/shell_parser.c` for command parsing
- [ ] Implement commands: `init`, `alloc`, `free`, `show`, `stat`, `frag`
- [ ] Basic error handling
- [ ] Update `main.c` to support `-i` flag

### Phase 2B.2: Algorithm & Display (1 hour)
- [ ] Implement `algo` command
- [ ] Implement `reset` command
- [ ] Implement `help` command
- [ ] Improve error messages with hints

### Phase 2B.3: Batch Mode (1 hour)
- [ ] Implement `-b <file>` flag
- [ ] Support stdin redirection (`< file.txt`)
- [ ] Handle comments in script files (`#`)
- [ ] Error reporting with line numbers

### Phase 2B.4: Polish (30 minutes)
- [ ] Add command aliases (`a` → `alloc`, `s` → `show`)
- [ ] Colorize shell prompt
- [ ] Tab completion (optional - advanced)
- [ ] Command history (optional - readline library)

### Phase 2B.5: Testing (1 hour)
- [ ] Create test suite in `tests/test_shell_commands.sh`
- [ ] Test all commands individually
- [ ] Test error handling
- [ ] Test batch mode
- [ ] Verify backward compatibility (menu mode still works)

**Total estimated time:** 5-6 hours

---

## 📝 INTEGRATION WITH EXISTING CODE

### Minimal Changes Required

```c
// main.c - Before (Phase 1)
int main(void) {
    HeapManager* heap = heap_init(1024);
    run_cli(heap);
    heap_destroy(heap);
    return 0;
}

// main.c - After (Phase 2B)
int main(int argc, char* argv[]) {
    /* Mode selection based on arguments */
    
    if (argc == 1) {
        /* Mode 1: Interactive Menu (default - backward compatible) */
        HeapManager* heap = heap_init(1024);
        run_cli(heap);
        heap_destroy(heap);
        return 0;
    }
    
    if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--interactive") == 0) {
        /* Mode 2: Interactive Shell */
        run_interactive_shell();
        return 0;
    }
    
    if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--batch") == 0) {
        /* Mode 3: Batch Mode */
        if (argc < 3) {
            fprintf(stderr, "Error: -b requires filename\n");
            return 1;
        }
        return run_batch_mode(argv[2]);
    }
    
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }
    
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        print_version();
        return 0;
    }
    
    /* Unknown argument */
    fprintf(stderr, "Unknown option: %s\n", argv[1]);
    fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
    return 1;
}
```

### No Breaking Changes
- ✅ `./memory_allocator` still works (menu mode)
- ✅ Existing code (`cli.c`, `heap_manager.c`, etc.) unchanged
- ✅ Only additions: `shell.c`, `shell_parser.c`
- ✅ All Phase 2A features (visualizer, colors) work in shell mode

---

## 🔗 RELATED DOCUMENTATION

- **[CONCEPT.md](CONCEPT.md)**: Theoretical background, project phases
- **[CHECKLIST.md](CHECKLIST.md)**: Implementation checklist with tasks
- **[PR.md](PR.md)**: Acceptance criteria and requirements
- **[PLATFORM.md](PLATFORM.md)**: Cross-platform compatibility notes

---

## ✅ ACCEPTANCE CRITERIA

### AC-SHELL1: Interactive Shell Launch
```bash
$ ./memory_allocator -i

Memory Allocator Interactive Shell v2.0
Type 'help' for commands, 'exit' to quit

allocator> 
```
**Expected:** Shell prompt displayed, waiting for input.

### AC-SHELL2: Stateful Session
```bash
allocator> init 1024
✓ Heap initialized: 1024 bytes

allocator> alloc P1:100
✓ Allocated 100 bytes for P1

allocator> show
[Memory map shows P1 allocated - heap state persisted]
```
**Expected:** Heap state persists across multiple commands.

### AC-SHELL3: Algorithm Selection
```bash
allocator> init 500
allocator> alloc A:100
allocator> alloc B:100
allocator> free A
allocator> algo bf
✓ Algorithm set to: Best Fit
allocator> alloc C:50
✓ Allocated 50 bytes for C at address 0
   (Best Fit chose 100-byte hole)
```
**Expected:** Algorithm switch affects subsequent allocations.

### AC-SHELL4: Error Handling
```bash
allocator> alloc P1:100
✗ Error: Heap not initialized. Use 'init <size>' first.

allocator> init 500
allocator> alloc P1:1000
✗ Error: Not enough memory

allocator> free P99
✗ Error: Process 'P99' not found
```
**Expected:** Graceful error messages, no crashes.

### AC-SHELL5: Batch Mode
```bash
$ ./memory_allocator -b test.txt
✓ Heap initialized: 1000 bytes
✓ Allocated 100 bytes for P1
[Memory map displayed]
```
**Expected:** Commands from file execute sequentially.

### AC-SHELL6: Backward Compatibility
```bash
$ ./memory_allocator

=============================================
      MEMORY ALLOCATION SIMULATOR            
=============================================

MAIN MENU:
1. Allocate Memory
...
```
**Expected:** Menu mode still works (Phase 1 unchanged).

### AC-SHELL7: Help System
```bash
$ ./memory_allocator --help
[Help text displayed]

allocator> help
[Command list displayed]
```
**Expected:** Help accessible from command-line and within shell.

---

## 🎓 LESSONS LEARNED & DESIGN RATIONALE

### Why Interactive Shell > One-Line Flags?

1. **Statefulness Problem:**
   - Each `./memory_allocator --alloc ...` = New process = Lost state
   - Shell keeps ONE process alive = Persistent heap

2. **Demo/Presentation:**
   - Professor can type commands step-by-step
   - Students see results immediately
   - Natural flow: command → result → next command

3. **Professional Appearance:**
   - Looks like real tools (`gdb`, `psql`, `redis-cli`)
   - Industry-standard REPL pattern
   - Clean, modern interface

4. **Flexibility:**
   - Easy to experiment: try → see result → try different approach
   - Reset and replay different scenarios
   - No need to remember long flag combinations

5. **Automation Still Possible:**
   - Pipe commands: `echo "init 1024" | ./memory_allocator -i`
   - Batch files: `./memory_allocator -b test.txt`
   - Best of both worlds

### Similar Tools Using REPL:
- **gdb**: `(gdb) break main`, `(gdb) run`
- **psql**: `postgres=# SELECT * FROM users;`
- **redis-cli**: `127.0.0.1:6379> SET key value`
- **python**: `>>> import sys`
- **mysql**: `mysql> SHOW DATABASES;`

All professional tools use interactive shells, not one-line flag chains!

---

**End of RECIPE_CMD.md**
