# RISCV Processor with and without clock gating
SystemC-based pipelined CPU simulation comparing normal execution vs clock-gated design to analyze performance and activity reduction.
# ⚙️ SystemC CPU: Clock Gating vs Normal Pipeline

This project implements a simple pipelined CPU in **SystemC** to compare:

- 🟢 Normal (always-active) pipeline
- 🔵 Clock-gated pipeline (stage-level activity control)

The goal is to demonstrate how **clock gating reduces unnecessary switching activity**, which is critical for power-efficient hardware design.

---

## 📌 Features

- 4-stage pipeline:
  - IF (Instruction Fetch)
  - ID (Instruction Decode)
  - EX (Execute)
  - MEM/WB (Memory + Writeback)
- Simple ALU (addition-based operations)
- Register file (32 registers)
- Instruction memory (256 words)
- Waveform-friendly signals for debugging
- Two implementations:
  - `cpu_normal.cpp` → baseline CPU
  - `cpu_gated.cpp` → clock-gated CPU

---

## 🧠 Key Concept: Clock Gating

Clock gating disables pipeline stages when they are not doing useful work.

### Without gating:
- All stages toggle every cycle
- Higher dynamic power consumption

### With gating:
- Idle stages are skipped
- Reduced switching activity
- More efficient design

---

---

## ⚙️ How It Works

### Pipeline Registers
Each stage communicates via structures:

- `IF_ID`
- `ID_EX`
- `EX_MEM`
- `MEM_WB`

Each contains:
- Data (PC, operands, results)
- Valid bit (`v`) → used for gating

---

### Clock Gating Logic

In `cpu_gated.cpp`, stages only execute when valid:

```cpp
if(id_ex.v) {
    // Execute stage runs
}
