# Memory Allocation Algorithms Simulator

A C-based simulation of memory allocation algorithms (First Fit, Next Fit, Best Fit, Worst Fit) for understanding OS heap memory management.

## 📋 Project Overview

**Course:** Systems Programming (Unix-like)  
**Language:** C (Standard C99+)  
**Platforms:** Ubuntu WSL & NetBSD (VirtualBox)

This project simulates how operating systems manage heap memory using different allocation strategies. It implements a virtual heap with a doubly-linked list data structure to demonstrate:
- Memory allocation (splitting blocks)
- Memory deallocation (coalescing adjacent free blocks)
- Fragmentation analysis
- Algorithm performance comparison

## 🎯 Features

### Core Features (MVP)
- ✅ 4 allocation algorithms: First Fit, Next Fit, Best Fit, Worst Fit
- ✅ Block splitting when allocating
- ✅ Block coalescing when freeing
- ✅ Interactive menu-driven CLI
- ✅ Memory map visualization
- ✅ Robust error handling

### Enhanced Features (Phase 2)
- Statistics dashboard
- Fragmentation analysis
- ANSI color support (auto-detected)

### Advanced Features (Phase 3 & UI/UX Enhancement)
- Script mode for automated testing
- Performance benchmarking
- Command-line interface
- 🖥️ **Interactive Web Visualizer (Apple iOS/macOS Theme - Desktop Dashboard)**:
  - Thiết kế chuẩn theo **Apple Human Interface Guidelines (HIG)** ứng dụng kỹ năng `ui-ux-pro-max-skill`, tối ưu bố cục dạng Dashboard màn hình rộng trên máy tính (không mô phỏng viền điện thoại).
  - **Bảng màu chủ đề chuẩn Apple iOS/macOS**: Sử dụng trọn vẹn bộ System Palette (`System Blue #007AFF`, `System Green #34C759`, `Indigo #5856D6`, `Orange #FF9500`, `Purple #AF52DE`, `Red #FF3B30`, `Teal #30B0C7`, `Yellow #FFCC00`) cùng hiệu ứng kính mờ (frosted glass) và thẻ bo góc mềm mại.
  - **Apple System Storage Bar**: Thanh trực quan hóa bộ nhớ liên tục trải rộng toàn màn hình, hiển thị phân bố khối nhớ vật lý trực quan kèm chú thích tiến trình và trạng thái phân đoạn.
  - **Cupertino Segmented Control**: Bộ điều khiển chuyển đổi giữa 4 thuật toán cấp phát (`First Fit`, `Next Fit`, `Best Fit`, `Worst Fit`) kèm mô tả cơ chế hoạt động.
  - **Mô phỏng Kịch bản Thực tế**: Chạy tự động các kịch bản tải của Hệ Điều Hành (*Random Mix, Worst-Case Fragmentation, Web Server Workload*), có nút Play/Pause, Step từng bước và điều chỉnh tốc độ.
  - **Chẩn đoán & Liên kết Đôi (DLL)**: Trực quan hóa danh sách liên kết đôi (Doubly-Linked List) dạng thẻ node tương tác, đo lường tỷ lệ phân đoạn ngoại và tỷ lệ thành công.
  - **Đồng bộ Dữ liệu C / JSON**: Xuất và nhập snapshot bộ nhớ tương thích 100% với file output của bản build C (`./memory_allocator`).
  - **Âm thanh phản hồi Haptic Apple**: Mô phỏng tiếng click xúc giác nhẹ khi bấm nút thông qua Web Audio API.

## 🛠️ Build Instructions

### Prerequisites

**Ubuntu WSL:**
```bash
sudo apt update
sudo apt install build-essential valgrind
```

**NetBSD (VirtualBox):**
```bash
pkgin update
pkgin install gcc gmake
```

### Compilation

**Ubuntu WSL:**
```bash
make clean
make
```

**NetBSD:**
```bash
gmake clean
gmake
```

**With strict warnings:**
```bash
make CFLAGS="-Wall -Wextra -Werror -std=c99 -O2"
```

### Running the Simulator

```bash
./memory_allocator
```

## 📖 Usage

### Interactive Menu

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

### Example Session

```bash
# Initialize heap
Choice: 1
Enter memory size (bytes): 1024

# Select algorithm
Choice: 2
Algorithms: 1=First Fit, 2=Next Fit, 3=Best Fit, 4=Worst Fit
Select: 1

# Allocate memory
Choice: 3
Enter Process ID: P1
Enter size (bytes): 100

# Show memory state
Choice: 5
# Displays memory map with all blocks

# Free memory
Choice: 4
Enter Process ID: P1
```

## 📂 Project Structure

```
memory-allocator/
├── src/
│   ├── core/               # Core data structures
│   │   ├── heap_manager.c/h
│   │   └── memory_block.c/h
│   ├── algorithms/         # Allocation algorithms
│   │   ├── first_fit.c/h
│   │   ├── next_fit.c/h
│   │   ├── best_fit.c/h
│   │   └── worst_fit.c/h
│   ├── utils/              # Utilities
│   │   ├── metrics.c/h
│   │   └── visualizer.c/h
│   ├── ui/                 # User interface
│   │   └── cli.c/h
│   └── main.c              # Entry point
├── tests/                  # Test files
├── scenarios/              # Test scenarios
├── docs/                   # Documentation
│   ├── CONCEPT.md
│   ├── PR.md
│   ├── PLATFORM.md
│   └── CHECKLIST.md
├── verify/                 # Setup verification logs
├── Makefile
├── .gitattributes          # LF line ending enforcement
└── README.md
```

## 🧪 Testing

### Manual Testing

Run through all acceptance criteria (AC1-AC8) defined in `docs/PR.md`:
- AC1: Initialization
- AC2-AC5: Algorithm correctness
- AC6: Block splitting
- AC7: Block coalescing
- AC8: Error handling

### Memory Leak Detection (Ubuntu WSL)

```bash
valgrind --leak-check=full --show-leak-kinds=all ./memory_allocator
```

### Cross-Platform Testing

Test on both Ubuntu WSL and NetBSD to ensure compatibility.

## 📊 Algorithms Overview

### First Fit
- **Strategy:** Find first free block large enough
- **Time Complexity:** O(n)
- **Pros:** Fast, simple
- **Cons:** Fragmentation at list head

### Next Fit
- **Strategy:** Continue search from last allocation point
- **Time Complexity:** O(n)
- **Pros:** Distributes allocations evenly
- **Cons:** Can fragment entire list

### Best Fit
- **Strategy:** Find smallest free block that fits
- **Time Complexity:** O(n)
- **Pros:** Minimizes wasted space
- **Cons:** Creates many small fragments

### Worst Fit
- **Strategy:** Find largest free block
- **Time Complexity:** O(n)
- **Pros:** Leaves large fragments for reuse
- **Cons:** Fragments large blocks quickly

## ⚙️ Technical Details

### Data Structures

```c
typedef struct MemoryBlock {
    size_t start_address;
    size_t size;
    int is_free;
    char process_id[32];
    struct MemoryBlock* next;
    struct MemoryBlock* prev;
} MemoryBlock;

typedef struct HeapManager {
    MemoryBlock* head;
    MemoryBlock* last_allocated;
    size_t total_size;
    size_t used_size;
    size_t free_size;
    AlgorithmType algorithm;
    int allocation_count;
    int deallocation_count;
    int failed_allocation_count;
} HeapManager;
```

### Memory Operations

**Splitting:** When allocating into a larger free block, split it into:
1. Allocated block (assigned to process)
2. Remaining free block

**Coalescing:** When freeing a block, merge with adjacent free blocks:
- Check previous block
- Check next block
- Merge all contiguous free blocks

## 📝 Known Limitations

- **Valgrind on NetBSD:** Limited support. Use Ubuntu WSL for memory leak detection.
- **ANSI Colors:** Depends on terminal emulator (auto-detected with fallback).
- **Performance:** Algorithms are O(n). Not optimized for production use.

## 🤝 Contributing

This is an academic project. For improvements:
1. Follow C99 standard
2. Use POSIX-compliant functions only
3. Test on both Ubuntu WSL and NetBSD
4. Run Valgrind to check for memory leaks
5. Ensure no compilation warnings with `-Wall -Wextra -Werror`

## 📚 Documentation

- **CONCEPT.md:** Design concepts and technical architecture
- **PR.md:** Product requirements (F1-F5, NF1-NF3, AC1-AC8)
- **PLATFORM.md:** Cross-platform compatibility analysis
- **CHECKLIST.md:** Development and testing checklist

## 🎓 Learning Outcomes

This project demonstrates:
- Dynamic memory management in C
- Linked list data structures
- Algorithm implementation and analysis
- Systems programming concepts
- Cross-platform development (POSIX)
- Memory leak prevention

## 📄 License

Academic project for educational purposes.

## ✨ Acknowledgments

Based on operating systems concepts from:
- "Operating System Concepts" - Silberschatz, Galvin, Gagne
- "Modern Operating Systems" - Andrew S. Tanenbaum

---

**Last Updated:** December 16, 2025  
**Version:** 1.0-dev  
**Status:** In Development
