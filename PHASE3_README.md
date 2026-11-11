# Phase 3: Quick Start Guide

## What's New in Phase 3?

✅ **Automated Benchmark System** - Compare all 4 algorithms with precise timing  
✅ **JSON Export** - Save memory state for analysis  
✅ **3 Test Scenarios** - Random, Worst-case, Web server patterns  
✅ **Performance Metrics** - Success rate, speed, fragmentation analysis

---

## Quick Commands

### Run Benchmark
```bash
# Single scenario
./memory_allocator --benchmark random

# All scenarios (recommended)
./memory_allocator --benchmark all
```

### Export Memory State
```bash
# In interactive shell
./memory_allocator -i
allocator> init 1024
allocator> alloc P1:100
allocator> export state.json
allocator> exit
```

### Batch Mode with Export
```bash
# Create script file
echo "init 2048
alloc P1:500
alloc P2:300
show
export final_state.json" > test.txt

# Run
./memory_allocator -b test.txt
```

---

## Example Output

### Benchmark Results
```
╔════════════════════════════════════════════════════════════════╗
║                 ALGORITHM COMPARISON                            ║
╠═══════════╦═══════════╦════════════╦═══════════╦════════╦══════╣
║ Algorithm ║  Success  ║  Avg Time  ║  Failed   ║  Peak  ║ Frag ║
║           ║   Rate    ║    (ms)    ║  Allocs   ║ (bytes)║ (%)  ║
╠═══════════╬═══════════╬════════════╬═══════════╬════════╬══════╣
║ First Fit ║  95.40%   ║  0.000234  ║     46    ║  8234  ║ 23.4 ║
║ Next Fit  ║  94.80%   ║  0.000198  ║     52    ║  8421  ║ 28.1 ║
║ Best Fit  ║  96.20%   ║  0.000456  ║     38    ║  8156  ║ 18.7 ║
║ Worst Fit ║  93.50%   ║  0.000421  ║     65    ║  8934  ║ 31.2 ║
╚═══════════╩═══════════╩════════════╩═══════════╩════════╩══════╝

🏆 Fastest: Next Fit
🏆 Most Reliable: Best Fit  
🏆 Least Fragmentation: Best Fit
```

### JSON Export
```json
{
  "timestamp": "2025-12-21 10:30:45",
  "algorithm": "Best Fit",
  "state": {
    "total_size": 1024,
    "used_size": 600,
    "free_size": 424,
    "utilization_percent": 58.59,
    "fragmentation_percent": 5.20
  },
  "blocks": [
    {"start": 0, "size": 100, "free": false, "process": "P1"},
    {"start": 100, "size": 200, "free": false, "process": "P2"},
    {"start": 300, "size": 724, "free": true}
  ]
}
```

---

## Test It!

### Windows PowerShell
```powershell
# Quick test
.\test_phase3.ps1
```

### Linux/WSL
```bash
# Compile
make clean && make

# Run benchmark
./memory_allocator --benchmark all

# Check results
ls -lh *.json
cat benchmark_random.json | jq .
```

---

## Files Generated

- `benchmark_random.json` - Random pattern results
- `benchmark_worst.json` - Worst-case fragmentation results  
- `benchmark_web.json` - Web server simulation results
- `test_export.json` - Manual export from shell

---

## For Your Report (Báo Cáo)

1. **Run full benchmark:**
   ```bash
   ./memory_allocator --benchmark all > results.txt
   ```

2. **Include in report:**
   - Comparison table (copy from console)
   - Winners summary
   - JSON data → Excel graphs

3. **Case study:**
   - Use worst-case scenario for fragmentation analysis
   - Show before/after states with JSON exports

---

## Troubleshooting

**"Command not found"**  
→ Run `make` first to compile

**"Slow benchmark"**  
→ Use release build: `make release`

**"JSON not created"**  
→ Check write permissions in current directory

**Compilation error**  
→ Requires POSIX system (Linux/WSL), not pure Windows

---

**Full documentation:** See [docs/PHASE3_BENCHMARK.md](docs/PHASE3_BENCHMARK.md)
