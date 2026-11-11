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
- 📱 **Interactive Web Visualizer (Apple iOS 18 / iPhone Theme)**:
  - Thiết kế chuẩn theo **Apple Human Interface Guidelines (HIG)** ứng dụng kỹ năng `ui-ux-pro-max-skill`.
  - **Dynamic Island**: Hiển thị trạng thái cấp phát bộ nhớ & thông báo thời gian thực với hiệu ứng mở rộng vật lý.
  - **iPhone Storage Bar**: Trực quan hóa heap bộ nhớ chuẩn phong cách mục *iPhone Storage* trong Settings của iOS.
  - **Cupertino Segmented Control**: Chuyển đổi thuật toán cấp phát mượt mà (First Fit, Next Fit, Best Fit, Worst Fit).
  - **Simulation & Scenarios**: Chạy mô phỏng kịch bản tải thực tế (*Random Mix, Worst-Case Fragmentation, Web Server Traffic*).
  - **Haptic Audio Feedback**: Hiệu ứng âm thanh xúc giác (haptic clicks) mô phỏng Apple Taptic Engine qua Web Audio API.
  - **Export & Import Snapshot**: Xuất/nhập dữ liệu trực tiếp tương thích với kết quả JSON và HTML của bản build C (`./memory_allocator`).
  - Hỗ trợ cả **iPhone Device View** (khung máy iPhone 16 Pro) và **Expanded Studio View** (màn hình rộng máy tính).

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
