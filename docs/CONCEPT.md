# Memory Allocation Algorithms Simulator
## Đề tài: Mô phỏng các thuật toán cấp phát bộ nhớ Heap

---

## MỤC TIÊU DỰ ÁN

### Mục tiêu chính
Xây dựng một công cụ mô phỏng (simulation) trực quan để hiểu sâu về cách hệ điều hành quản lý bộ nhớ Heap thông qua 4 thuật toán cơ bản:
- **First Fit**: Tìm vùng trống đầu tiên đủ lớn
- **Next Fit**: Tìm vùng trống tiếp theo từ vị trí cuối cùng
- **Best Fit**: Tìm vùng trống vừa khít nhất
- **Worst Fit**: Tìm vùng trống lớn nhất

### Giá trị mang lại
- Hiểu rõ cơ chế Memory Management của OS
- So sánh hiệu năng các thuật toán
- Quan sát hiện tượng External/Internal Fragmentation
- Công cụ giảng dạy cho môn Operating Systems

---

## Ý TƯỞNG VÀ GIẢI PHÁP

### 1. **Mô hình hóa Heap Memory**

```
Heap Memory = Danh sách liên kết các Memory Block (Doubly Linked List)
┌─────────────────────────────────────────┐
│ Block 1 │ Block 2 │ Block 3 │ Block 4  │
│ [FREE]  │ [USED]  │ [FREE]  │ [USED]   │
└─────────────────────────────────────────┘
```

**Cấu trúc dữ liệu (Theo yêu cầu NF3 - Bắt buộc dùng Linked List):**
```c
typedef enum {
    FIRST_FIT,
    NEXT_FIT,
    BEST_FIT,
    WORST_FIT
} AlgorithmType;

typedef struct MemoryBlock {
    size_t start_address;    // Địa chỉ bắt đầu (offset)
    size_t size;             // Kích thước block (bytes)
    int is_free;             // 1 = FREE, 0 = ALLOCATED (dùng int thay bool cho C99)
    char process_id[32];     // ID tiến trình (string, VD: "P1", "P2")
    struct MemoryBlock* next;
    struct MemoryBlock* prev;
} MemoryBlock;

typedef struct HeapManager {
    MemoryBlock* head;              // Đầu danh sách
    MemoryBlock* last_allocated;    // Con trỏ cho Next Fit
    size_t total_size;              // Tổng dung lượng heap
    size_t used_size;               // Đã sử dụng
    size_t free_size;               // Còn trống
    AlgorithmType algorithm;        // Thuật toán hiện tại
    int allocation_count;           // Số lần cấp phát thành công
    int deallocation_count;         // Số lần thu hồi
    int failed_allocation_count;    // Số lần cấp phát thất bại
} HeapManager;
```

**LƯU Ý QUAN TRỌNG (Theo PR.md):**
- **KHÔNG sử dụng mảng cố định** để mô phỏng bộ nhớ
- Phải dùng `malloc()` để cấp phát động các node MemoryBlock
- Mỗi block là một node trong linked list

### 2. **Kiến trúc hệ thống - 3 tầng**

```
┌──────────────────────────────────────────┐
│   PRESENTATION LAYER (CLI/GUI)          │
│   - Hiển thị trực quan                   │
│   - Nhận lệnh người dùng                 │
│   - Thống kê & báo cáo                   │
└──────────────────────────────────────────┘
           ↕
┌──────────────────────────────────────────┐
│   BUSINESS LOGIC LAYER                   │
│   - Memory allocation algorithms         │
│   - Memory deallocation                  │
│   - Fragmentation analysis               │
│   - Performance metrics                  │
└──────────────────────────────────────────┘
           ↕
┌──────────────────────────────────────────┐
│   DATA LAYER                             │
│   - Heap data structure                  │
│   - Block management                     │
│   - History tracking                     │
└──────────────────────────────────────────┘
```

---

## CƠ CHẾ KỸ THUẬT

### A. Thuật toán First Fit
**Cơ chế:**
1. Duyệt danh sách từ đầu
2. Chọn block FREE đầu tiên có `size >= requested_size`
3. Split block nếu dư thừa quá lớn

**Ưu điểm:** Nhanh, đơn giản
**Nhược điểm:** Fragmentation ở đầu list

```c
MemoryBlock* first_fit(HeapManager* heap, size_t size) {
    MemoryBlock* current = heap->head;
    
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            return allocate_block(current, size);
        }
        current = current->next;
    }
    return NULL; // Out of memory
}
```

### B. Thuật toán Next Fit
**Cơ chế:**
1. Bắt đầu từ vị trí cuối cùng đã cấp phát
2. Tìm kiếm theo vòng tròn (circular)
3. Phân bố đều fragmentation

**Ưu điểm:** Phân bố tải đều hơn First Fit
**Nhược điểm:** Cần track last position

```c
MemoryBlock* next_fit(HeapManager* heap, size_t size) {
    MemoryBlock* current = heap->last_allocated_position;
    MemoryBlock* start = current;
    
    do {
        if (current->is_free && current->size >= size) {
            heap->last_allocated_position = current;
            return allocate_block(current, size);
        }
        current = (current->next != NULL) ? current->next : heap->head;
    } while (current != start);
    
    return NULL;
}
```

### C. Thuật toán Best Fit
**Cơ chế:**
1. Duyệt toàn bộ danh sách
2. Chọn block có size nhỏ nhất mà vẫn đủ
3. Giảm thiểu lãng phí

**Ưu điểm:** Tối ưu sử dụng bộ nhớ
**Nhược điểm:** Chậm (O(n)), tạo nhiều fragment nhỏ

```c
MemoryBlock* best_fit(HeapManager* heap, size_t size) {
    MemoryBlock* current = heap->head;
    MemoryBlock* best = NULL;
    size_t min_diff = SIZE_MAX;
    
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            size_t diff = current->size - size;
            if (diff < min_diff) {
                min_diff = diff;
                best = current;
            }
        }
        current = current->next;
    }
    
    return (best != NULL) ? allocate_block(best, size) : NULL;
}
```

### D. Thuật toán Worst Fit
**Cơ chế:**
1. Duyệt toàn bộ danh sách
2. Chọn block lớn nhất
3. Sau khi split vẫn còn block lớn để dùng

**Ưu điểm:** Fragment lớn, dễ tái sử dụng
**Nhược điểm:** Lãng phí, phân mảnh nhanh

```c
MemoryBlock* worst_fit(HeapManager* heap, size_t size) {
    MemoryBlock* current = heap->head;
    MemoryBlock* worst = NULL;
    size_t max_size = 0;
    
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (current->size > max_size) {
                max_size = current->size;
                worst = current;
            }
        }
        current = current->next;
    }
    
    return (worst != NULL) ? allocate_block(worst, size) : NULL;
}
```

---

## HƯỚNG TRÌNH BÀY & VISUALIZATION

### 1. **CLI Mode - Text-based Visualization**

```
=== HEAP MEMORY MAP ===
Total: 1024 KB | Used: 456 KB | Free: 568 KB | Fragmentation: 23.4%

0KB    100KB   200KB   300KB   400KB   500KB   600KB   700KB   800KB   900KB   1024KB
├──────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┤
[P1:80][ FREE ][P2:120][FREE:40][P3:216][    FREE: 568 KB               ]

Legend:
  [Px:size] - Allocated to Process x with size KB
  [FREE]    - Free block
  
Statistics:
  Algorithm: First Fit
  Total Allocations: 3
  Failed Allocations: 0
  Average Allocation Time: 0.003ms
  Largest Free Block: 568 KB
  Number of Free Blocks: 3
  External Fragmentation: 23.4%
```

### 2. **Menu-Driven Interface (Theo PR.md)**

```
========================================
  MEMORY ALLOCATION SIMULATOR
========================================
Current Algorithm: First Fit
Total Memory: 1024 KB | Used: 0 KB | Free: 1024 KB

MENU:
  1. Set Total Memory Size
  2. Select Algorithm (First/Next/Best/Worst Fit)
  3. Allocate Memory (Process ID + Size)
  4. Free Memory (Process ID)
  5. Show Memory Map
  6. Exit

Enter your choice: _
```

**Command-line Alternative (mvp + optional):**
```bash
Commands:
  alloc <pid> <size>      - Allocate memory to process
  free <pid>              - Free memory of process
  switch <algorithm>      - Switch algorithm (ff/nf/bf/wf)
  show                    - Show memory map
  stats                   - Show detailed statistics
  compact                 - Defragmentation (Optional)
  script <file>           - Run commands from file
  reset                   - Reset heap
  help                    - Show this help
  quit                    - Exit simulator
```

### 3. **Colors với ANSI Codes (Optional - Enhancement)**

```c
// Thêm màu sắc cho terminal output (Linux/Unix)
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_RESET   "\x1b[0m"

void show_memory_map_colored() {
    printf(ANSI_RED "[P1:80]" ANSI_RESET);
    printf(ANSI_GREEN "[FREE:50]" ANSI_RESET);
    printf(ANSI_RED "[P2:120]" ANSI_RESET "\n");
}
```

**Lưu ý:** Không làm GUI (ncurses, SDL, GTK...) vì:
- ❌ Tốn thời gian implementation
- ❌ Xa rời cơ chế memory allocation gốc
- ❌ Không cần thiết cho môn Lập trình hệ thống
- ✅ Terminal CLI là đủ để demo và đạt điểm cao

---

## METRICS & EVALUATION

### Các chỉ số đánh giá:

1. **External Fragmentation**
   ```
   External_Frag = (Total_Free_Space - Largest_Free_Block) / Total_Free_Space × 100%
   ```

2. **Internal Fragmentation**
   ```
   Internal_Frag = (Allocated_But_Unused) / Total_Allocated × 100%
   ```

3. **Allocation Success Rate**
   ```
   Success_Rate = Successful_Allocations / Total_Requests × 100%
   ```

4. **Average Allocation Time**
5. **Memory Utilization**
   ```
   Utilization = Used_Memory / Total_Memory × 100%
   ```

---

## TESTING STRATEGY

### 1. Unit Tests
- Test từng thuật toán riêng lẻ
- Test edge cases (full memory, zero size, etc.)
- Test coalescing (gộp block kề nhau)

### 2. Benchmark Scenarios

**Scenario A: Random Allocation/Deallocation**
```
- 1000 random allocations (10-500KB)
- 500 random frees
- Compare algorithms
```

**Scenario B: Worst Case**
```
- Allocate nhiều block nhỏ
- Free các block chẵn
- Allocate block lớn → test fragmentation
```

**Scenario C: Real-world Simulation**
```
- Mô phỏng pattern của web server
- Short-lived và long-lived objects
```

### 3. Visualization Tests
- Export memory state to JSON
- Generate graphs (fragmentation over time)
- Compare algorithms side-by-side

---

## STRUCTURE DỰ ÁN

```
memory-allocator/
│
├── src/
│   ├── core/
│   │   ├── heap_manager.c/h       # Core heap management
│   │   ├── memory_block.c/h       # Block operations
│   │   └── allocator.c/h          # Algorithm implementations
│   │
│   ├── algorithms/
│   │   ├── first_fit.c/h
│   │   ├── next_fit.c/h
│   │   ├── best_fit.c/h
│   │   └── worst_fit.c/h
│   │
│   ├── utils/
│   │   ├── metrics.c/h            # Statistics & metrics
│   │   ├── visualizer.c/h         # CLI/Terminal visualization
│   │   └── logger.c/h             # Logging (optional)
│   │
│   ├── ui/
│   │   └── cli.c/h                # Command-line interface
│   │
│   └── main.c                     # Entry point
│
├── tests/
│   ├── test_allocator.c
│   ├── test_scenarios.c
│   └── benchmark.c
│
├── scenarios/                     # Pre-defined test cases
│   ├── random.txt
│   ├── worst_case.txt
│   └── web_server.txt
│
├── docs/
│   ├── ALGORITHM_ANALYSIS.md
│   ├── API_REFERENCE.md
│   └── USER_GUIDE.md
│
├── Makefile
├── README.md
└── CONCEPT.md                     # This file
```

---

## ROADMAP PHÁT TRIỂN

### Phase 1: Core Requirements (MVP)
**Timeline:** 1-2 tuần
- [ ] **Data structures:** MemoryBlock linked list, HeapManager
- [ ] **Implement 4 algorithms:** First/Next/Best/Worst Fit
- [ ] **Allocation với Splitting:** Cắt block khi dư thừa
- [ ] **Deallocation với Coalescing:** Gộp blocks liền kề sau mỗi free
- [ ] **Input validation:** Không crash khi nhập sai (size âm, PID không tồn tại)
- [ ] **Menu-driven CLI:** Menu 1-6 theo PR (Set Size, Select Algo, Alloc, Free, Show, Exit)

**Acceptance Criteria:** Pass AC1-AC8 trong PR.md

### Phase 2: Enhanced Visualization
**Timeline:** 3-5 ngày
- [ ] ASCII memory map đẹp hơn (aligned, borders)
- [ ] ANSI colors cho terminal output (optional)
- [ ] Real-time statistics display
- [ ] Fragmentation report (% phân mảnh)
- [ ] Command history

### Phase 3: Advanced Features (optional - không làm cũng được)
**Timeline:** 1 tuần
- [ ] **Script Mode:** Đọc file lệnh để test tự động
- [ ] Benchmark scenarios với report
- [ ] Export memory state to file/JSON
- [ ] Performance comparison table
- [ ] Command-line mode (alternative to menu)

### Phase 4: Extra Credit (Optional - Nếu còn thời gian)
- [ ] Memory leak detection với Valgrind integration
- [ ] Defragmentation/Compaction feature
- [ ] Extended algorithms (Buddy System, Slab)
- [ ] Multi-level visualization (timeline view)

---

## KIẾN THỨC CẦN ÁP DỤNG

### Lý thuyết
- Operating Systems: Memory Management
- Data Structures: Linked Lists, Trees
- Algorithms: Search, Sort, Optimization
- Complexity Analysis: Time & Space

### Kỹ năng lập trình
- **C (Standard C99+)**: 
  - Pointers, Dynamic Memory (`malloc`, `free`)
  - Structures and Linked Lists
  - String handling (`strcpy`, `strcmp`, `sprintf`)
  - **KHÔNG sử dụng C++** (theo yêu cầu NF2)
  - **KHÔNG làm GUI libraries** (ncurses, SDL, GTK...)
- **Unix System Programming**: 
  - Biên dịch với `gcc` trên Linux/Unix/WSL
  - Terminal I/O (`printf`, `scanf`, `fgets`)
  - ANSI escape codes cho colors (optional)
  - File I/O cho script mode
- **Tools**: 
  - GCC compiler: `gcc -Wall -Wextra -std=c99 -o simulator main.c`
  - GDB debugger cho debugging
  - Valgrind memory checker (optional): `valgrind --leak-check=full ./simulator`
  - Makefile cho build automation

---

## TÀI LIỆU THAM KHẢO

1. **Books:**
   - "Operating System Concepts" - Silberschatz (Chapter 9)
   - "Modern Operating Systems" - Tanenbaum (Chapter 3)
   - "The C Programming Language" - K&R

2. **Papers:**
   - "A Fast Algorithm for First-Fit Storage Allocation"
   - "Memory Allocation Algorithms: A Comparative Study"

3. **Online Resources:**
   - OSDev Wiki: Memory Management
   - GNU libc malloc implementation
   - jemalloc, tcmalloc source code

---

## Ý TƯỞNG MỞ RỘNG

### Phase 2: Enhanced Visualization & Professional CLI ✅ **COMPLETED**

#### 2.1. Enhanced ASCII Visualization
**Trạng thái:** ✅ **Hoàn thành** (December 20, 2025)

```
═══════════════════════════════════════════════════════════════════════════
                        MEMORY LAYOUT
═══════════════════════════════════════════════════════════════════════════
Scale: 0     100     200     300     400     500     600     700     800     900     1000
       ┌────────────────────────────────────────────────────────────┐
       │██████░░░░░░░░░░░░░░░░░░█████████░░░░░░░░░░░░░░░░░░░░░░░░░░░│
       └────────────────────────────────────────────────────────────┘
       Legend:  █ Allocated    ░ Free
```

**Tính năng đã triển khai:**
- ✅ Box-drawing characters (UTF-8: ╔═╗║╚╝┌─┐│└┘)
- ✅ Proportional memory bar với scale ruler
- ✅ ANSI colors với auto-detection (`isatty()`)
- ✅ Color-coded severity levels (green/yellow/red)
- ✅ Real-time stats bar trước mỗi menu
- ✅ Fragmentation analysis với interpretation

#### 2.2. Interactive Command-Line Shell ✅ **COMPLETED** (December 21, 2025)
**Tài liệu:** [RECIPE_CMD.md](RECIPE_CMD.md)  
**Test Plan:** [TEST_SHELL.md](TEST_SHELL.md)

**REPL-based Command Interface (Stateful Session):**
```bash
# Mode 1: Interactive Menu (mặc định - backward compatible)
./memory_allocator

# Mode 2: Interactive Shell (NEW - RECOMMENDED for demo)
./memory_allocator -i

# Enter shell với command prompt:
allocator> init 1024
✓ Heap initialized: 1024 bytes

allocator> alloc P1:100
✓ Allocated 100 bytes for P1

allocator> show
[Memory map displayed - heap persists!]

allocator> algo bf
✓ Algorithm set to: Best Fit

allocator> alloc P2:200
✓ Allocated 200 bytes for P2

allocator> frag
[Fragmentation report displayed]

allocator> exit
Goodbye!
```

**Lợi ích so với one-line flags:**
- ✅ **Stateful** - Heap tồn tại suốt session (1 process duy nhất)
- ✅ **Demo-friendly** - Gõ từng command → Xem kết quả ngay lập tức
- ✅ **Step-by-step** - Hoàn hảo cho instructor demonstration
- ✅ **Professional** - Giống `gdb`, `psql`, `redis-cli` (industry standard)
- ✅ **Scriptable** - Vẫn pipe được: `./memory_allocator < script.txt`
- ✅ **No flag hell** - Không cần nhớ chuỗi flag dài

**Commands hỗ trợ:**
```
init <size>           Initialize heap
alloc <PID>:<size>    Allocate memory
free <PID>            Free memory
algo <ff|nf|bf|wf>    Set algorithm
show [mode]           Display memory map (default/simple/detail)
stat                  Display statistics
frag                  Fragmentation report
reset                 Reset heap
help                  Command list
exit                  Exit shell
```

**Implementation Status:**
- ✅ Core REPL loop với command prompt
- ✅ Command parser (`init`, `alloc`, `free`, `show`, `stat`, `frag`, `algo`, `reset`)
- ✅ Error handling per command với helpful messages
- ✅ Batch mode: `./memory_allocator -b script.txt` và stdin redirection
- ✅ Help system: `--help`, `--version`, `help` command
- ✅ Testing: AC-SHELL1 to AC-SHELL7 (7/7 manual tests)
- ✅ Enhanced visualization: `show` (beautiful), `show simple`, `show detail`
- ✅ Command aliases: `a`, `s`, `f`, `q`, `st`, `fr`
- ✅ Backward compatibility: Menu mode vẫn hoạt động

**Files Created:**
- `src/ui/shell.c` - REPL loop implementation (420 lines)
- `src/ui/shell.h` - Shell interface header
- `src/utils/shell_parser.c` - Command parser (244 lines)
- `src/utils/shell_parser.h` - Parser interface
- `scenarios/test_shell.txt` - Test script
- `scenarios/demo_visualization.txt` - Demo script
- `docs/TEST_SHELL.md` - Comprehensive test plan

**Ưu tiên implementation:** ✅ ALL PRIORITIES COMPLETED
```
✅ Priority 1 (Core REPL):     REPL loop, basic commands
✅ Priority 2 (Features):      algo, stat, frag, reset commands
✅ Priority 3 (Batch):         -b flag, stdin redirection, comments
✅ Priority 4 (Polish):        Aliases, colored prompt, enhanced show modes
```

**Tại sao chọn Interactive Shell thay vì CLI flags?**

| Vấn đề | One-line Flags | Interactive Shell |
|--------|----------------|-------------------|
| **Statefulness** | ❌ Mỗi lần chạy = process mới = mất state | ✅ 1 process = state tồn tại |
| **Demo** | ❌ Không thấy intermediate results | ✅ Perfect - từng bước rõ ràng |
| **Complexity** | ❌ `--init 1024 --alloc P1:100 --alloc P2:200 --free P1 --show` (quá dài) | ✅ 5 commands ngắn gọn |
| **Professional** | ⚠️ Unusual pattern | ✅ Industry standard (gdb, psql) |

**Chi tiết đầy đủ:** Xem [docs/RECIPE_CMD.md](RECIPE_CMD.md)

---

### Phase 3: Benchmark & Performance Analysis ✅ **COMPLETED** (December 21, 2025)

**Status:** ✅ **PRODUCTION READY**  
**Documentation:** [PHASE3_BENCHMARK.md](PHASE3_BENCHMARK.md)

#### 3.1. **Benchmark Framework**
Automated performance testing with high-precision timing:

```bash
# Run single scenario
./memory_allocator --benchmark random
./memory_allocator --benchmark worst
./memory_allocator --benchmark web

# Run all scenarios and compare
./memory_allocator --benchmark all
```

**Features:**
- ✅ Nanosecond-precision timer (`clock_gettime`)
- ✅ 4 algorithms compared automatically
- ✅ Statistics: success rate, avg/max time, fragmentation
- ✅ 3 predefined scenarios (random, worst-case, web server)
- ✅ Beautiful comparison tables with winners summary
- ✅ JSON export for external analysis

**Output Example:**
```
╔════════════════════════════════════════════════════════════════╗
║                 ALGORITHM COMPARISON                            ║
╠═══════════╦═══════════╦════════════╦═══════════╦════════╦══════╣
║ Algorithm ║  Success  ║  Avg Time  ║  Failed   ║  Peak  ║ Frag ║
║           ║   Rate    ║    (ms)    ║  Allocs   ║ (bytes)║ (%)  ║
╠═══════════╬═══════════╬════════════╬═══════════╬════════╬══════╣
║ First Fit ║  95.40%   ║  0.000234  ║     46    ║  8234  ║ 23.4 ║
║ Best Fit  ║  96.20%   ║  0.000456  ║     38    ║  8156  ║ 18.7 ║
╚═══════════╩═══════════╩════════════╩═══════════╩════════╩══════╝

🏆 Fastest: Next Fit | Most Reliable: Best Fit | Least Frag: Best Fit
```

#### 3.2. **JSON Export System**
Export memory state for analysis:

```bash
# In interactive shell
allocator> init 1024
allocator> alloc P1:100
allocator> export state.json
✓ Memory state exported to: state.json
```

**Export Functions:**
- `export_heap_to_json()` - Summary (size, utilization, fragmentation)
- `export_blocks_to_json()` - Detailed block list
- `export_stats_to_json()` - Statistics summary
- `export_snapshot_to_json()` - Complete snapshot

**JSON Example:**
```json
{
  "timestamp": "2025-12-21 10:30:45",
  "algorithm": "Best Fit",
  "state": {
    "total_size": 10000,
    "used_size": 4500,
    "fragmentation_percent": 12.34
  },
  "blocks": [
    {"start": 0, "size": 100, "free": false, "process": "P1"},
    {"start": 100, "size": 900, "free": true}
  ]
}
```

#### 3.3. **Benchmark Scenarios**

**Scenario 1: Random Mix** (`SCENARIO_RANDOM`)
- 10,000 bytes heap, 1,000 operations
- 60% alloc / 40% free probability
- General-purpose workload test

**Scenario 2: Worst Case** (`SCENARIO_WORST_CASE`)
- 10,000 bytes heap, 200 operations
- Pattern: Allocate → Free alternating → Large alloc
- Fragmentation stress test

**Scenario 3: Web Server** (`SCENARIO_WEB_SERVER`)
- 50,000 bytes heap, 2,000 operations
- Mix of short-lived (requests) and long-lived (sessions)
- Realistic server simulation

**Batch Scripts Created:**
- `scenarios/benchmark_random.txt`
- `scenarios/benchmark_worst_case.txt`
- `scenarios/benchmark_webserver.txt`

#### 3.4. **Implementation Summary**

**Files Created (7 files, ~1,500 lines):**
- `src/utils/benchmark.h` - Benchmark interface (90 lines)
- `src/utils/benchmark.c` - Benchmark implementation (600 lines)
- `src/utils/export.h` - Export interface (20 lines)
- `src/utils/export.c` - Export implementation (300 lines)
- `scenarios/benchmark_*.txt` - 3 test scenarios (145 lines)
- `docs/PHASE3_BENCHMARK.md` - Complete documentation (400 lines)

**Files Modified (4 files):**
- `src/main.c` - Added `--benchmark` mode, run_benchmark() function
- `src/ui/shell.c` - Added `export` command handler
- `src/utils/shell_parser.c/h` - Added CMD_EXPORT
- `Makefile` - Added benchmark.o, export.o compilation

**Testing:**
- ✅ Benchmark runs complete (all scenarios)
- ✅ JSON export working (all formats)
- ✅ Shell export command functional
- ✅ Comparison tables accurate
- ✅ Timer precision validated

**Performance:**
- Random scenario: ~0.5-1 second (1000 ops × 4 algos)
- Worst case: ~0.2-0.3 seconds (200 ops × 4 algos)
- Web server: ~1-2 seconds (2000 ops × 4 algos)
- Full suite: ~3-5 seconds (all scenarios × 4 algos)

**Use Cases:**
1. Performance comparison for báo cáo
2. Fragmentation case studies
3. Algorithm selection guidance
4. Automated testing
5. Data collection for graphs/charts

---

### Phase 4: Demo Visualization & Presentation Tools ⚠️ **PLANNED** (Optional)

**Status:** 📋 **PLANNING STAGE** - Not yet implemented  
**Estimated Time:** 1-2 hours implementation  
**Priority:** Low (nice-to-have for demo, not required for grade)

#### 4.1. **Slow Script Mode (Animation Demo)**

**Purpose:** Automated demo mode with step-by-step visualization for presentations

**Problem với batch mode hiện tại:**
```bash
./memory_allocator -b script.txt
# → Tất cả commands chạy NGAY LẬP TỨC
# → Không thấy được từng bước thay đổi
# → Khó follow cho người xem demo
```

**Solution: Slow Demo Mode:**
```bash
./memory_allocator --demo script.txt --delay 2

# Screen shows each step with pause:
═══════════════════════════════════════════════════════════
Executing: init 1000
═══════════════════════════════════════════════════════════
✓ Heap initialized: 1000 bytes

Scale: 0    100   200   300   400   500   600   700   800   900   1000
┌────────────────────────────────────────────────────────────┐
│░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│
└────────────────────────────────────────────────────────────┘

[Pausing 2 seconds...]

═══════════════════════════════════════════════════════════
Executing: alloc P1:100
═══════════════════════════════════════════════════════════
✓ Allocated 100 bytes for P1

Scale: 0    100   200   300   400   500   600   700   800   900   1000
┌────────────────────────────────────────────────────────────┐
│█████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│
└────────────────────────────────────────────────────────────┘

[Pausing 2 seconds...]

... (continues for each command)
```

**Features:**
1. **Configurable Delay:**
   - `--delay 0.5` - Fast demo (0.5 seconds)
   - `--delay 2` - Normal demo (2 seconds, recommended)
   - `--delay 5` - Slow demo (5 seconds, for detailed explanation)

2. **Visual Feedback:**
   - Command display với border
   - Memory visualization after EACH command
   - Pause message with countdown (optional)
   - Optional screen clear between commands

3. **Script Enhancements:**
   - Support `# comment` lines (skipped)
   - Support `pause` command (wait for Enter)
   - Support `clear` command (clear screen)
   - Support `say <text>` command (print narration)

**Demo Script Example:**
```bash
# scenarios/demo_presentation.txt
# Demo for instructor presentation

say "=== Phase 1: Initialization ==="
init 1000
pause

say "=== Phase 2: Allocations ==="
alloc P1:100
alloc P2:200
alloc P3:150

say "=== Phase 3: Fragmentation ==="
free P2
show detail

say "=== Phase 4: Coalescing ==="
free P1
free P3
show
```

**Implementation Plan:**

**Files to Modify:**
1. `src/main.c` - Add `--demo` and `--delay` flag parsing
2. `src/ui/shell.c` - Modify `run_batch_mode()` to support demo mode
3. `src/utils/shell_parser.c/h` - Add CMD_PAUSE, CMD_CLEAR, CMD_SAY

**Code Structure:**
```c
// New function in shell.c
void run_demo_mode(const char* filename, float delay_seconds) {
    FILE* f = fopen(filename, "r");
    char line[256];
    
    while (fgets(line, sizeof(line), f)) {
        // Skip comments
        if (line[0] == '#') continue;
        
        // Show command
        printf("\n");
        printf("═══════════════════════════════════════════════════\n");
        printf("Executing: %s", line);
        printf("═══════════════════════════════════════════════════\n");
        
        // Execute command
        ParsedCommand cmd = parse_shell_command(line);
        execute_command(&cmd);
        
        // Show memory visualization
        if (g_shell_heap) {
            printf("\n");
            draw_memory_bar(g_shell_heap);
            printf("\n");
        }
        
        // Pause with countdown
        printf("[Pausing %.1f seconds...]\n", delay_seconds);
        usleep((useconds_t)(delay_seconds * 1000000));
        
        // Optional: Clear screen for next command
        // printf("\033[2J\033[H"); // ANSI clear screen
    }
    
    fclose(f);
}
```

**Usage Examples:**
```bash
# Basic demo với 2 giây delay
./memory_allocator --demo scenarios/demo_presentation.txt

# Fast demo (0.5s)
./memory_allocator --demo scenarios/quick_demo.txt --delay 0.5

# Slow detailed demo (5s)
./memory_allocator --demo scenarios/detailed_demo.txt --delay 5

# No delay (instant, like batch mode)
./memory_allocator --demo scenarios/test.txt --delay 0
```

**Benefits:**
- ✅ Perfect cho instructor presentations
- ✅ Clear step-by-step visualization
- ✅ Controllable speed
- ✅ Easy to prepare demo scripts
- ✅ No manual intervention needed
- ✅ Reproducible demos

**Implementation Checklist:**
- [ ] Add `--demo` flag parsing in main.c (5 min)
- [ ] Add `--delay <seconds>` flag with default 2.0 (5 min)
- [ ] Create `run_demo_mode()` function (20 min)
- [ ] Add sleep/usleep between commands (5 min)
- [ ] Show memory visualization after each command (10 min)
- [ ] Support comment lines starting with `#` (5 min)
- [ ] Optional: Add CMD_PAUSE, CMD_SAY commands (15 min)
- [ ] Create demo scripts (3-4 scripts) (15 min)
- [ ] Test with different delay values (10 min)
- [ ] Update documentation (10 min)

**Total Estimated Time:** 60-90 minutes

---

#### 4.2. **Real-time Memory Monitor** ❌ **NOT RECOMMENDED**

**Description:** Live-updating display similar to `htop`/`top`

**Why NOT recommended for this project:**
1. ⚠️ **Complexity:** Requires ncurses library or complex ANSI sequences
2. ⚠️ **Threading:** Non-blocking input needs threads or async I/O
3. ⚠️ **Scope Creep:** Shifts focus from memory algorithms to terminal UI
4. ⚠️ **Cross-platform:** May break NetBSD compatibility
5. ⚠️ **Time:** 4-6 hours minimum, high risk of bugs
6. ⚠️ **Value:** Low educational value for OS course

**Decision:** Skip this feature. Slow Demo Mode provides better demo value with 10x less effort.

---

### Phase 3: Script Mode & Advanced Features (Optional - NOT IMPLEMENTED)

#### 3.2. **Fragmentation Report** ✅ **Completed in Phase 2**
```
╔═══════════════════════════════════════════════════════════════════════╗
║              FRAGMENTATION ANALYSIS                                    ║
╠═══════════════════════════════════════════════════════════════════════╣
║  Total Free Space:        750    bytes                                 ║
║  Largest Free Block:      500    bytes                                 ║
║  Number of Free Blocks:   2                                          ║
║  External Fragmentation:  33.3% (Moderate)                             ║
╚═══════════════════════════════════════════════════════════════════════╝
  
Formula: (Total_Free - Largest_Free) / Total_Free × 100%
```

#### 3.3. **Advanced Algorithms (Optional)**
- **Buddy System:** Binary tree structure, power-of-2 sizes only
- **Slab Allocator:** Object caching (như Linux kernel)
- **Garbage Collection:** Mark and Sweep simulation
- **Defragmentation/Compaction:** Di chuyển blocks để gộp free space

**Lưu ý:** KHÔNG làm GUI/graphics. Focus vào algorithms và terminal CLI.

---

## CHECKLIST BÁO CÁO & DEMO

### Báo cáo nên bao gồm:

**1. Phần lý thuyết:**
- [ ] Mô tả chi tiết 4 thuật toán với flowchart
- [ ] Phân tích độ phức tạp (Time & Space Complexity)
- [ ] So sánh ưu/nhược điểm giữa các thuật toán
- [ ] Giải thích Splitting và Coalescing

**2. Phần thiết kế:**
- [ ] Sơ đồ cấu trúc dữ liệu (Linked List)
- [ ] Kiến trúc hệ thống (3-layer)
- [ ] Flowchart cho từng thuật toán

**3. Phần implementation:**
- [ ] Source code có comment đầy đủ (tiếng Anh hoặc tiếng Việt)
- [ ] Makefile hoặc build instructions
- [ ] Đáp ứng tất cả Functional Requirements (F1-F5)
- [ ] Đáp ứng tất cả Non-Functional Requirements (NF1-NF3)

**4. Phần testing:**
- [ ] Pass tất cả 8 Acceptance Criteria (AC1-AC8)
- [ ] Test cases với input/output mẫu
- [ ] Screenshot hoặc video demo
- [ ] Test error handling (nhập sai dữ liệu)

**5. Phần kết quả:**
- [ ] So sánh performance giữa các thuật toán
- [ ] Kết quả fragmentation với các scenarios khác nhau
- [ ] Biểu đồ (nếu có)

**6. Phần kết luận:**
- [ ] Lessons learned
- [ ] Challenges faced & solutions
- [ ] Future improvements

### Demo Checklist:
- [ ] Chạy được trên Linux/Unix/WSL
- [ ] Compile không có warnings: `gcc -Wall -Wextra -std=c99`
- [ ] Không memory leaks (check với Valgrind)
- [ ] Menu hoạt động mượt mà
- [ ] Visualization rõ ràng, dễ hiểu nên bao gồm:

- [ ] Coalescing (BẮT BUỘC theo PR F4):** 
   - Phải gộp các free blocks kề nhau **NGAY LẬP TỨC** sau mỗi lệnh free
   - Kiểm tra cả block trước và block sau
   - Ví dụ: `[Free:10] - [P1:20] - [Free:30]` → Free P1 → `[Free:60]`

2. **Splitting Policy:** 
   - Luôn split block khi `block_size > requested_size`
   - Không cần threshold phức tạp cho MVP
   - Ví dụ: Alloc 10 vào block Free 100 → `[P1:10] - [Free:90]`

3. **Error Handling (BẮT BUỘC theo PR NF1):**
   ```c
   // Phải validate input để không crash:
   - Size <= 0 → Báo lỗi "Invalid size"
   - Process ID không tồn tại khi free → Báo lỗi "Process not found"
   - Không đủ memory → Báo lỗi "Not enough memory"
   - Avoid segmentation fault bằng null pointer checks
   ```

4. **Memory Alignment (Optional cho MVP):** 
   - Có thể bỏ qua cho version đầu tiên
   - Nếu làm: align to 8 bytes

### Về Testing (Theo PR Acceptance Criteria):
**Phải pass tất cả 8 test cases sau:**

| Test ID | Scenario | Expected Result |
|---------|----------|-----------------|
| AC1 | Init với size 1000 | Hiển thị 1 block Free 1000 |
| AC2 | First Fit: Free blocks (50, 100), alloc 20 | Chọn block 50 |
| AC3 | Best Fit: Free blocks (100, 30), alloc 20 | Chọn block 30 |
| AC4 | Worst Fit: Free blocks (100, 30), alloc 20 | Chọn block 100 |
| AC5 | Next Fit: Sau alloc vào block 1 | Lần sau tìm từ block 1 trở đi |
| AC6 | Splitting: Alloc 10 vào Free 100 | Result: [P1:10] + [Free:90] |
| AC7 | Coalescing: Free P1 giữa 2 Free blocks | Gộp thành 1 block duy nhất |
| AC8 | Error: Alloc size > total free | Báo lỗi, không crash |

### Về Performance:
1. First/Next Fit: O(n) - n là số blocks
2. Best/Worst Fit: O(n) - luôn phải duyệt hết
3. Cho MVP không cần optimize, focus vào correctness
### Về Implementation:
1. **Memory Alignment**: Đảm bảo alignment (thường là 8 hoặc 16 bytes)
2. **Coalescing**: Gộp các free blocks kề nhau ngay lập tức
3. **Splitting Policy**: Split block khi dư thừa > threshold (ví dụ 32 bytes)
4. **Boundary Tags**: Giúp coalescing hiệu quả hơn

### Về Testing:
1. Test với Valgrind để phát hiện memory leaks
2. Stress test với allocation/deallocation patterns phức tạp
3. Đảm bảo không có undefined behavior

### Về Performance:
1. First/Next Fit: O(n) - n là số blocks
2. Best/Worst Fit: O(n) - luôn phải duyệt hết
3. Có thể tối ưu bằng segregated free lists (nâng cao)

---

## LƯU Ý QUAN TRỌNG VỀ `malloc()`

### Phân biệt rõ ràng:

**BẠN SỬ DỤNG `malloc()` (đơn giản):**
```c
// malloc() là CÔNG CỤ có sẵn - chỉ cần gọi và dùng:
MemoryBlock* node = malloc(sizeof(MemoryBlock));  // ← Đơn giản!
node->size = 100;
node->is_free = 1;
// malloc() lo việc cấp phát RAM thật, bạn KHÔNG cần biết chi tiết bên trong
```

**BẠN IMPLEMENT LOGIC ALLOCATOR (đây mới là bài tập):**
```c
// ĐÂY là phần BẠN viết code - thuật toán allocation:
MemoryBlock* my_first_fit(HeapManager* heap, size_t size) {
    // Duyệt linked list
    // Tìm block phù hợp  
    // Splitting & Coalescing
    // Quản lý fragmentation
    // → LOGIC NÀY LÀ TRỌNG TÂM CỦA DỰ ÁN
}
```

### Tại sao dùng `malloc()` là hoàn hảo:

| ✅ Lý do | Giải thích |
|---------|-----------|
| **Đơn giản & Tin cậy** | Stable, không lỗi, test kỹ bởi GNU/Linux |
| **Tiết kiệm thời gian** | Không lãng phí hàng trăm giờ làm low-level memory |
| **Đúng mục tiêu** | Học **THUẬT TOÁN allocation**, không làm OS kernel |
| **Standard C99** | Đáp ứng yêu cầu NF2 của bài tập |
| **Focus đúng chỗ** | Năng lượng dành cho First/Best/Worst Fit logic |

### Hiểu đúng về "malloc() phức tạp":

> **Note trong TÀI LIỆU THAM KHẢO đề cập:** "GNU libc malloc implementation, jemalloc, tcmalloc..."

**Ý nghĩa:**
- Internal implementation của `malloc()` trong libc **RẤT PHỨC TẠP** (bins, arenas, mmap fallback, thread caching...)
- Nhưng đó là **TÀI LIỆU ĐỂ THAM KHẢO**, không phải để bạn implement
- **BẠN CHỈ DÙNG** `malloc()` như một black box: input size → output pointer
- **BẠN FOCUS VÀO:** Xây dựng allocator simulation với 4 thuật toán

### Tương tự:

```
BẠN DÙNG printf()     → Không cần biết printf() xử lý format string thế nào
BẠN DÙNG qsort()      → Không cần biết quicksort trong libc implement ra sao
BẠN DÙNG malloc()     → Không cần biết malloc() quản lý heap thế nào

BẠN VIẾT:
  ├─ First Fit algorithm   ← TRỌNG TÂM
  ├─ Best Fit algorithm    ← TRỌNG TÂM  
  ├─ Splitting logic       ← TRỌNG TÂM
  └─ Coalescing logic      ← TRỌNG TÂM
```

### Kết luận:

**Project này dạy bạn:**
- ✅ Hiểu các thuật toán allocation (First/Next/Best/Worst Fit)
- ✅ Quản lý linked list và memory blocks
- ✅ Handle fragmentation
- ✅ Implement splitting & coalescing

**Project này KHÔNG yêu cầu:**
- ❌ Viết lại malloc() từ đầu
- ❌ Làm system calls (mmap, brk, sbrk)
- ❌ Implement low-level memory management của OS

---
