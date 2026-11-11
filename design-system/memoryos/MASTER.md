# Design System Master File - Apple Native Experience (iOS / iPhone Theme)

> **LOGIC:** When building or styling interfaces for MemoryOS Simulator, follow this Master specification.
> Based on Apple Human Interface Guidelines (HIG) and `ui-ux-pro-max` Apple Native Experience intelligence.

---

**Project:** MemoryOS - Memory Allocation Algorithms Simulator  
**Style:** Apple Native Experience / iOS 18 Design Language  
**Target Platforms:** Responsive Web (iPhone Device View + iPad/Desktop Adaptive View) & ANSI CLI  
**Generated:** 2026-11-11  

---

## 1. Design Principles & HIG Alignment

1. **Content-First Hierarchy:** Memory blocks, heap status, and process diagnostics take center stage with high legibility.
2. **Apple Inset Grouped Styling:** Rounded cards (`border-radius: 16px` to `20px`) with subtle borders and dividers.
3. **Tactile Vibrancy & Frosted Glass:** Translucent navigation bars and Dynamic Island with `backdrop-filter: blur(20px) saturate(180%)`.
4. **Cupertino Controls:** Segmented controls with sliding pill selection, iOS switches (toggles), action sheets, and SF Symbols style iconography.
5. **iPhone Storage Paradigm:** Heap visualization mirrors the iconic iOS Settings "iPhone Storage" bar with multi-colored category blocks and clean legend.

---

## 2. Color Palette & Semantic Tokens

### Light Appearance (`color-scheme: light`)
| Token | Hex / Value | Usage |
|-------|-------------|-------|
| `--color-bg-system` | `#F2F2F7` | System Grouped Background |
| `--color-surface` | `#FFFFFF` | Grouped Card / Inset Surface |
| `--color-surface-secondary` | `#E5E5EA` | Secondary surface / inactive pill |
| `--color-label-primary` | `#000000` | Primary text |
| `--color-label-secondary` | `#8E8E93` | Subtitles, labels, metadata |
| `--color-label-tertiary` | `#C7C7CC` | Disabled, hints |
| `--color-separator` | `rgba(60, 60, 67, 0.12)` | Inset divider line |

### Dark Appearance (`color-scheme: dark`)
| Token | Hex / Value | Usage |
|-------|-------------|-------|
| `--color-bg-system` | `#000000` | OLED Pure Black Background |
| `--color-surface` | `#1C1C1E` | Secondary System Grouped Surface |
| `--color-surface-secondary` | `#2C2C2E` | Tertiary Surface / inactive pill |
| `--color-label-primary` | `#FFFFFF` | Primary text |
| `--color-label-secondary` | `#8E8E93` | Subtitles, labels, metadata |
| `--color-label-tertiary` | `#48484A` | Disabled, hints |
| `--color-separator` | `rgba(84, 84, 88, 0.36)` | Inset divider line |

### Apple HIG System Accents
| Token | Hex | Usage |
|-------|-----|-------|
| `--apple-blue` | `#007AFF` | Primary interactive, links, First Fit badge |
| `--apple-green` | `#34C759` | Free memory, success, Next Fit badge |
| `--apple-indigo` | `#5856D6` | Best Fit badge, system metrics |
| `--apple-orange` | `#FF9500` | Worst Fit badge, warning states |
| `--apple-pink` | `#FF2D55` | Accent processes |
| `--apple-purple` | `#AF52DE` | Memory fragmentation metrics |
| `--apple-red` | `#FF3B30` | Destructive action, failed allocs, high frag |
| `--apple-teal` | `#30B0C7` | Process highlight, secondary stats |
| `--apple-yellow` | `#FFCC00` | Moderate fragmentation status |

---

## 3. Typography & Hierarchy

- **System UI Font:** `-apple-system, BlinkMacSystemFont, "SF Pro Display", "SF Pro Text", "SF Pro", -apple-system-subheadline, "Helvetica Neue", Arial, sans-serif`
- **Monospace Code / Address Font:** `ui-monospace, "SF Mono", Menlo, Monaco, Consolas, "Liberation Mono", monospace`

| Role | Size | Weight | Letter-spacing |
|------|------|--------|----------------|
| Large Title | 34px | Bold (700) | 0.37px |
| Title 1 | 28px | Bold (700) | 0.36px |
| Title 2 | 22px | Bold (700) | 0.35px |
| Title 3 | 20px | Semibold (600) | 0.38px |
| Headline | 17px | Semibold (600) | -0.41px |
| Body | 17px | Regular (400) | -0.41px |
| Callout | 16px | Regular (400) | -0.32px |
| Subheadline | 15px | Regular (400) | -0.24px |
| Footnote | 13px | Regular (400) | -0.08px |
| Caption 1 | 12px | Medium (500) | 0.00px |
| Caption 2 | 11px | Regular (400) | 0.06px |

---

## 4. Components Specifications

### Dynamic Island
- Compact pill: `border-radius: 20px`, pure black `#000000` with subtle border, animated expansion on allocation / deallocation / warnings.
- Expanded mode: shows process badge, size, fragmentation delta, and status icon.

### iPhone Storage Bar (Memory Visualizer)
- Multi-segment rounded bar (`height: 24px`, `border-radius: 8px`).
- Proportional block widths with minimum visible threshold (6px).
- Distinct pastel/vibrant Apple colors for allocated processes; diagonal subtle stripes for free blocks.
- Interactive tooltip displaying: `Process ID`, `Start Offset`, `End Offset`, `Size (KB / Bytes)`, `State`.

### Cupertino Segmented Control
- Outer container: `background: rgba(118, 118, 128, 0.12)`, `border-radius: 9px`, `padding: 2px`.
- Active segment: sliding white pill (dark mode: `#636366`), `box-shadow: 0 3px 8px rgba(0,0,0,0.12)`.
- Smooth slide transition: `300ms cubic-bezier(0.25, 1, 0.5, 1)`.

### Inset Grouped Lists
- Rounded cards (`border-radius: 14px`), inset from container edges by 16px.
- Internal rows separated by hairline divider indented 16px from the left.

---

## 5. Animations & Haptics
- **Spring curve:** `cubic-bezier(0.32, 0.72, 0, 1)` (Apple standard spring).
- **Duration:** 250ms - 350ms.
- **Audio/Haptics:** Synthesized micro-clicks for segmented control changes and allocation operations.
