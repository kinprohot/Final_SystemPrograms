# PRODUCT REQUIREMENTS DOCUMENT (PRD)
**Đề tài:** Mô phỏng các thuật toán cấp phát bộ nhớ (Memory Allocation Simulation)
**Môn học:** Lập trình hệ thống (Unix-like)

---

### 1. Functional Requirements (Yêu cầu Chức năng)
Đây là các tính năng cốt lõi mà phần mềm **bắt buộc** phải thực hiện được.

#### F1. Khởi tạo bộ nhớ (Initialization)
*   Hệ thống cho phép khởi tạo một vùng nhớ giả lập (Virtual Heap) với kích thước tổng cộng do người dùng nhập (hoặc mặc định, ví dụ: 1024 bytes).
*   Ban đầu, toàn bộ vùng nhớ là một khối (block) trạng thái `FREE`.

#### F2. Lựa chọn thuật toán (Algorithm Selection)
Người dùng phải chọn được 1 trong 4 chế độ hoạt động trước khi thực hiện cấp phát:
1.  **First Fit:** Tìm khối Free đầu tiên đủ lớn.
2.  **Next Fit:** Tìm khối Free tiếp theo từ vị trí cấp phát cuối cùng.
3.  **Best Fit:** Tìm khối Free nhỏ nhất nhưng vừa đủ (để tiết kiệm không gian).
4.  **Worst Fit:** Tìm khối Free lớn nhất (để để lại phần thừa lớn).

#### F3. Cấp phát bộ nhớ (Allocation - `malloc` simulation)
*   Người dùng nhập lệnh cấp phát kèm: `Process ID` (tên tiến trình) và `Size` (kích thước).
*   Hệ thống tìm khối nhớ phù hợp dựa trên thuật toán đang chọn.
*   **Cơ chế Splitting (Cắt):** Nếu khối Free tìm được lớn hơn kích thước yêu cầu, hệ thống phải cắt khối đó thành 2 phần:
    *   Phần 1: Được gán cho Process ID (Trạng thái: BUSY).
    *   Phần 2: Phần dư còn lại (Trạng thái: FREE).
*   Nếu không tìm thấy khối phù hợp: Thông báo lỗi "Not enough memory".

#### F4. Thu hồi bộ nhớ (Deallocation - `free` simulation)
*   Người dùng nhập `Process ID` cần giải phóng.
*   Hệ thống chuyển trạng thái khối nhớ đó từ `BUSY` sang `FREE`.
*   **Cơ chế Coalescing (Gộp):** Sau khi Free, hệ thống phải kiểm tra các khối liền kề (trước và sau). Nếu chúng cũng là `FREE`, phải gộp chung lại thành một khối lớn hơn (tránh phân mảnh giả tạo).

#### F5. Hiển thị trạng thái (Status Reporting)
*   Hiển thị danh sách các khối nhớ hiện tại theo thứ tự địa chỉ.
*   Thông tin hiển thị mỗi khối: `[Start Address] - [Size] - [Status: Free/P_ID]`.

---

### 2. Non-Functional Requirements (Yêu cầu Phi chức năng)
Các yêu cầu về chất lượng và môi trường hệ thống.

*   **NF1. Tính bền vững (Robustness):** Chương trình không được crash (Segmentation fault) khi người dùng nhập sai dữ liệu (VD: nhập size âm, nhập ID không tồn tại để xóa).
*   **NF2. Môi trường (Environment):** Code phải viết bằng **C (Standard C99 trở lên)** và biên dịch/chạy tốt trên môi trường **Linux/Unix** (Ubuntu, CentOS, hoặc WSL) sử dụng `gcc`.
*   **NF3. Cấu trúc dữ liệu:** Bắt buộc sử dụng **Linked List** (Danh sách liên kết) để quản lý các khối nhớ (Nodes). Không dùng mảng cố định để mô phỏng động.

---

### 3. User Interface Requirements (Yêu cầu Giao diện)
Do là lập trình hệ thống, ưu tiên giao diện dòng lệnh (CLI) rõ ràng.

*   **UI1. Menu Driven:** Có menu vòng lặp để người dùng thao tác liên tục.
    ```text
    1. Set Total Memory Size
    2. Select Algorithm (Current: First Fit)
    3. Allocate Memory
    4. Free Memory
    5. Show Memory Map
    6. Exit
    ```
*   **UI2. Visualization (Trực quan hóa):** Khi hiển thị bộ nhớ (Show Memory Map), nên có biểu diễn dạng đồ họa ASCII để dễ nhìn.
    *   Ví dụ: `[ P1: 100KB ]---[ FREE: 50KB ]---[ P2: 200KB ]`

---

### 4. Acceptance Criteria (Tiêu chí Nghiệm thu)
Đây là bảng kiểm (Checklist) để xác định sản phẩm đạt điểm tối đa.

| ID | Tiêu chí (Criteria) | Điều kiện đạt (Pass Condition) |
|:---|:---|:---|
| **AC1** | **Khởi tạo** | Chạy chương trình, nhập tổng dung lượng 1000. Hiển thị ra 1 block Free 1000. |
| **AC2** | **First Fit Logic** | Có 2 khối Free (A: 50, B: 100). Cấp phát 20. Chương trình chọn khối A (vì gặp A trước). |
| **AC3** | **Best Fit Logic** | Có 2 khối Free (A: 100, B: 30). Cấp phát 20. Chương trình chọn khối B (vì 30 gần 20 nhất). |
| **AC4** | **Worst Fit Logic** | Có 2 khối Free (A: 100, B: 30). Cấp phát 20. Chương trình chọn khối A (vì 100 lớn nhất). |
| **AC5** | **Next Fit Logic** | Sau khi cấp phát vào khối 1, lần cấp phát tiếp theo bắt đầu tìm từ khối 1 trở đi (không quay lại đầu danh sách). |
| **AC6** | **Tính năng Splitting** | Cấp phát 10 vào khối Free 100. Kết quả phải là: [P1: 10] và [Free: 90]. Tổng size vẫn đủ. |
| **AC7** | **Tính năng Coalescing** | Trạng thái: `[Free: 10] - [P1: 20] - [Free: 30]`. Thực hiện Free P1. Kết quả phải gộp thành 1 khối duy nhất: `[Free: 60]`. |
| **AC8** | **Xử lý lỗi** | Cố tình cấp phát size lớn hơn Free còn lại -> Chương trình báo lỗi, không crash. |

---

### 5. Phase 2 Enhancements (Implemented - December 2025)

#### Optional Feature #1: Fragmentation Report ✅ **COMPLETED**
- **Implementation:** `src/ui/visualizer.c` - `show_fragmentation_report()`
- **Formula:** `(Total_Free - Largest_Free_Block) / Total_Free × 100%`
- **Features:**
  - Box-bordered display with UTF-8 characters
  - Color-coded severity: Green (<30%), Yellow (30-60%), Red (>60%)
  - Shows: Total free space, largest free block, number of free blocks, fragmentation %
  - Interpretation guide for users
- **Test Results:** 13/13 assertions passed (100%)

#### Optional Feature #2: Enhanced Visualization ✅ **COMPLETED**
- **Real-time Stats Bar:**
  - Always displayed before menu
  - Memory usage percentage with color-coding
  - Current algorithm name
  - Allocation/deallocation/fail counters
  - Number of free blocks
  
- **ASCII Memory Bar:**
  - Proportional horizontal bar chart
  - Scale rulers (0, 100, 200, ..., 1000)
  - Visual blocks: █ (allocated), ░ (free)
  - Color-coded: Red (allocated), Green (free)
  - Block details list with addresses

- **ANSI Color Support:**
  - Auto-detection via `isatty(STDOUT_FILENO)`
  - Toggle option in menu (Option 7)
  - Graceful fallback for non-color terminals

#### Optional Feature #3: Command-Line Flags ⚠️ **PENDING** (Phase 2B)
**Documentation:** [RECIPE_CMD.md](RECIPE_CMD.md)

**Overview:** Professional Linux-style CLI with flags (backward compatible)

**Essential Flags:**
```bash
# Interactive mode (default - backward compatible)
./memory_allocator
./memory_allocator --menu

# Direct operations
./memory_allocator --init 1024 --alloc P1:100 --show
./memory_allocator --algo bf --alloc P1:100 --alloc P2:200 --stat --frag

# Help system
./memory_allocator --help
./memory_allocator --version

# Automation-friendly
./memory_allocator --quiet --init 1024 --alloc P1:100 --stat > output.txt
```

**Benefits:**
- ✅ 70-90% reduction in user interactions (6-12 menu steps → 1 command)
- ✅ Scriptable & automatable (ideal for CI/CD)
- ✅ Professional interface following Linux conventions (`ls`, `grep`, `gcc`)
- ✅ Backward compatible (no flags → traditional menu)
- ✅ Perfect for benchmarking & testing

**Implementation Status:**
- [ ] Core parser with `getopt_long()`
- [ ] Essential flags: `--init`, `--alloc`, `--free`, `--show`
- [ ] Algorithm & stats: `--algo`, `--stat`, `--frag`
- [ ] Quality features: `--quiet`, `--color`, `--no-color`
- [ ] Testing: AC-CLI1 to AC-CLI6

**Full specification:** See [docs/RECIPE_CMD.md](RECIPE_CMD.md) for complete flag definitions, usage examples, and acceptance criteria.

---

### 6. Phase 3 Advanced Features ✅ **COMPLETED** (December 2025)

#### Optional Feature #4: Benchmark System
- **Description:** Automated performance testing and comparison
- **Status:** ✅ **COMPLETED** (December 21, 2025)
- **Features:**
  - High-precision nanosecond timer
  - 4 algorithms auto-compared
  - 3 benchmark scenarios (random, worst-case, web server)
  - JSON export for analysis
  - Performance comparison tables
- **Implementation:** `src/utils/benchmark.c/h`, `src/utils/export.c/h`
- **Documentation:** [docs/PHASE3_BENCHMARK.md](PHASE3_BENCHMARK.md)
- **Usage:** `./memory_allocator --benchmark <scenario>`

---

### 7. Phase 4 Presentation Features ⚠️ **PLANNED** (Optional)

#### Optional Feature #5: Slow Demo Mode (Animation)
- **Description:** Step-by-step animated demo mode with configurable delays
- **Status:** ⚠️ **PLANNED** - Design complete, not yet implemented
- **Priority:** Low (optional enhancement for demos)
- **Estimated Time:** 1-2 hours implementation

**Motivation:**
Current batch mode executes all commands instantly, making it hard to follow during presentations. Slow demo mode pauses between commands with visualization, perfect for instructor demonstrations.

**Syntax:**
```bash
# Run demo with 2-second delay between commands
./memory_allocator --demo scenarios/demo_presentation.txt --delay 2

# Fast demo (0.5 seconds)
./memory_allocator --demo scenarios/quick_demo.txt --delay 0.5

# Slow detailed demo (5 seconds)
./memory_allocator --demo scenarios/detailed_demo.txt --delay 5
```

**Features:**
1. **Configurable Delay:** `--delay <seconds>` (default: 2.0)
2. **Visual Feedback:** Shows command, executes, displays memory map, pauses
3. **Script Enhancements:**
   - Support `# comment` lines (documentation)
   - Support `pause` command (wait for Enter)
   - Support `say <text>` command (narration)
4. **Auto-visualization:** Memory bar shown after each command

**Demo Script Example:**
```bash
# scenarios/demo_presentation.txt
say "=== Demonstrating First Fit Algorithm ==="
init 1000
alloc P1:100
alloc P2:200
alloc P3:150
pause

say "=== Demonstrating Fragmentation ==="
free P2
show detail

say "=== Demonstrating Coalescing ==="
free P1
free P3
show
```

**Benefits:**
- ✅ Perfect for instructor presentations
- ✅ Clear step-by-step visualization
- ✅ Controllable speed (0.5s to 5s)
- ✅ No manual intervention needed
- ✅ Reproducible demos
- ✅ Easy to prepare demo scripts

**Acceptance Criteria:**

| ID | Criteria | Pass Condition |
|:---|:---|:---|
| **AC-DEMO1** | **Flag Parsing** | `--demo file.txt --delay 2` correctly parsed |
| **AC-DEMO2** | **Step Execution** | Each command pauses with visualization |
| **AC-DEMO3** | **Delay Control** | Different `--delay` values work correctly |
| **AC-DEMO4** | **Comment Support** | Lines starting with `#` are skipped |
| **AC-DEMO5** | **Visualization** | Memory bar shown after each command |
| **AC-DEMO6** | **Script Compatibility** | Works with existing batch scripts |

**Implementation Checklist:**
- [ ] Add `--demo` flag parsing (5 min)
- [ ] Add `--delay <seconds>` with default 2.0 (5 min)
- [ ] Create `run_demo_mode()` function (20 min)
- [ ] Add `sleep()`/`usleep()` between commands (5 min)
- [ ] Show memory visualization after each (10 min)
- [ ] Support `#` comment lines (5 min)
- [ ] Optional: CMD_PAUSE, CMD_SAY (15 min)
- [ ] Create demo scripts (15 min)
- [ ] Testing (10 min)
- [ ] Documentation (10 min)

**Total Time:** 60-90 minutes

**Not Recommended:**
- ❌ **Real-time Monitor (htop-style):** Too complex (ncurses, threading), low educational value, 4-6 hours, high risk. Better to focus on algorithms.

---

### 8. Summary of Optional Features

| Feature | Phase | Status | Effort | Value | Recommendation |
|---------|-------|--------|--------|-------|----------------|
| Fragmentation Report | Phase 2 | ✅ Done | 2h | High | ✅ Essential |
| Enhanced Visualization | Phase 2 | ✅ Done | 4h | High | ✅ Essential |
| Interactive Shell | Phase 2B | ✅ Done | 8h | Very High | ✅ Essential |
| Benchmark System | Phase 3 | ✅ Done | 4h | High | ✅ Excellent bonus |
| Slow Demo Mode | Phase 4 | ⚠️ Planned | 1-2h | Medium | ⚠️ Optional (demo help) |
| Real-time Monitor | Phase 4 | ❌ Rejected | 4-6h | Low | ❌ Not worth it |

**Current Grade Estimate:**
- Phase 1 only: 8.5-9/10
- Phase 1 + 2A + 2B: 9.5/10
- Phase 1 + 2A + 2B + 3: 10/10+
- With Slow Demo Mode: Bonus points for presentation

---

Đây là bộ khung yêu cầu đầy đủ để bạn bắt tay vào code. Bạn nên copy phần này vào báo cáo đồ án ở mục "Phân tích yêu cầu" (Requirement Analysis).