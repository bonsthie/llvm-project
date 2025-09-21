# H2BLB Custom Backend Learning
This is a **toy architecture** created to learn how to build an LLVM backend using GISel.
The main code of this backend is available in [llvm/lib/Target/H2BLB](llvm/lib/Target/H2BLB).

Currently, the backend provides:

* Integration with **clang** and **llc** via `-target h2blb`
* Support for **custom builtins** in clang (`__builtin_h2blb...()`)
* A small **constant propagation pass** specific to H2BLB (both old and new pass managers)
* **Register definitions and handling** (caller/callee saved, reserved registers)
* **Instruction definitions and handling** (scheduling, properties, pseudo-instructions)
* **Instruction encoding/mapping** for object file emission (via the MC layer)
* **GlobalISel support**:
  * Legalization of generic operations
  * Register bank assignment
  * Instruction selection into target opcodes
  * Call lowering (arguments, returns)
  * Target-specific combines and optimizations

# Main Classes Explained

This is separated into 4 parts: the main lib `Target/H2BLB` and its sub-libs `AsmParser/`, `MCTargetDesc/`, `TargetInfo/`.

LLVM splits target support into: **TargetInfo** (identity), **TargetMachine** (codegen config), **Subtarget** (per-CPU features), **TargetLowering** (IR→DAG), and **MCTargetDesc/AsmParser** (MC layer).

(LLVM IR → SelectionDAG → MachineInstr → MCInst → encoding)

---

## Ownership & Dependencies

```
TargetInfo
└─ (no ownership; registers target identity: triple, DataLayout, macros, builtins)

TargetMachine
├─ owns: pass/pipeline config (old/new PM hooks)
├─ creates/holds: Subtarget (often per-function)
└─ provides: TargetTransformInfo (TTI) → uses Subtarget for cost info (no ownership)

Subtarget
├─ owns: TargetInstrInfo     (InstrInfo)
├─ owns: TargetRegisterInfo  (RegInfo)
├─ owns: TargetFrameLowering (FrameLowering)
├─ owns: SelectionDAGTargetInfo (SelectionDAGInfo)
└─ owns: TargetLowering      (IR → SelectionDAG lowering rules)
    └─ TargetLowering may query TTI for costs (TTI still owns nothing)

TTI (TargetTransformInfo)
└─ uses: Subtarget → InstrInfo / SchedModel / RegInfo for cost queries
   (TTI does NOT own InstrInfo; the Subtarget does)
```

MC layer (constructed by the target’s MC *factory*; typically not owned by Subtarget):

```
MCTargetDesc (H2BLBMCTargetDesc)
├─ creates: MCAsmInfo
├─ creates: MCRegisterInfo
├─ creates: MCInstrInfo
├─ creates: MCSubtargetInfo
├─ creates: MCInstPrinter
└─ creates: MCCodeEmitter

Usage (no ownership here):
• AsmPrinter / assembler use: MCAsmInfo, MCInstrInfo, MCRegisterInfo, MCSubtargetInfo
• Disassembler uses: MCInstrInfo, MCRegisterInfo, MCSubtargetInfo
• MCInstPrinter uses: MCInstrInfo
• MCCodeEmitter uses: MCInstrInfo + MCSubtargetInfo to produce encodings
• Lifetimes usually managed by MCContext/AsmPrinter/Assembler, not by Subtarget
```

---

## main

### [H2BLB{.h/.cpp}](llvm/lib/Target/H2BLB/H2BLB.h) (H2BLBTargetInfo : public TargetInfo)

Registers the base information about the target:

* DataLayout
* Target defines (ex: `__H2BLB__`)
* Target builtins (ex: `__builtin_h2blb...()/@llvm.h2blb...`) generated with [BuiltinsH2BLB.td](clang/include/clang/Basic/BuiltinsH2BLB.td)
* Type of `__builtin_va_list` that should be used

### [H2BLBTargetMachine{.h/.cpp}](llvm/lib/Target/H2BLB/H2BLBTargetMachine.h) (H2BLBTargetMachine : public CodeGenTargetMachineImpl)

Target-specific code generation configuration object:

* feature string (ex: `-Xclang -target-feature -Xclang +sse2,-sse`)
* sets default behavior for the target (ex: how math instructions are interpreted)
* relocation model (how symbols are accessed)
* codegen model (ex: size settings of the binary)
* optimisation level
* provides per-function `Subtarget` access through `getSubtargetImpl`
* hooks for adding target-specific passes to the old and new PM (e.g. via `registerPipelineStartEPCallback`)

### [H2BLBTargetLowering{.h/.cpp}](llvm/lib/Target/H2BLB/H2BLBTargetLowering.h) (H2BLBTargetLowering : public TargetLowering)

Defines how LLVM IR operations are legalised and lowered into SelectionDAG nodes, including custom instruction patterns.

### [H2BLBSubtarget{.h/.cpp}](llvm/lib/Target/H2BLB/H2BLBSubtarget.h) (H2BLBSubtarget : public TargetSubtargetInfo)

Encapsulates CPU/feature configuration and **owns** per-target components (`InstrInfo`, `FrameLowering`, `RegInfo`, `TargetLowering`).

### [h2blbtargettransforminfo{.h/.cpp}](llvm/lib/Target/H2BLB/h2blbtargettransforminfo.h) (H2BLBTTIImpl : public llvm::BasicTTIImplBase<H2BLBTTIImpl>)

Provides cost models to LLVM’s optimisers (vectoriser, unroller, inliner). **Does not own target data, but queries it through Subtarget/InstrInfo.**

### IR Pass Manager

* [H2BLBConstantPropagationPass.cpp](llvm/lib/Target/H2BLB/H2BLBConstantPropagationPass.cpp) : small constant propagation pass for H2BLB
* [H2BLBPassRegistry.def](llvm/lib/Target/H2BLB/H2BLBPassRegistry.def) : registry of the CP pass for the old PM

### [H2BLBFrameLowering{.h/.cpp}](llvm/lib/Target/H2BLB/H2BLBFrameLowering.h) (H2BLBFrameLowering : public TargetFrameLowering)

Implements stack frame layout and prologue/epilogue emission for function calls (saving/restoring callee-saved registers, stack pointer adjustments, etc.).

### [H2BLBRegisterInfo{.h/.cpp}](llvm/lib/Target/H2BLB/H2BLBRegisterInfo.h)

Provides information about registers (caller/callee-saved, reserved, stack pointer, frame pointer) — mostly generated from `H2BLBGenRegisterInfo.inc`.

---

## TargetInfo

### [TargetInfo/H2BLBTargetInfo{.h/.cpp}](llvm/lib/Target/H2BLB/TargetInfo/H2BLBTargetInfo.h)

`LLVMInitializeH2BLBTargetInfo` registers the target with the singleton retrievable via `getTheH2BLBTarget()`.

---

## MCTargetDesc

MC stands for *Machine Code*. This library registers and builds MC-layer components (code emitter, asm info, instruction printer). These are used both by the backend and by assembler/disassembler tools.

### [H2BLBMCTargetDesc{.h/.cpp}](llvm/lib/Target/H2BLB/MCTargetDesc/H2BLBMCTargetDesc.h)

`LLVMInitializeH2BLBTargetMC` initialises all MC tools based on a `RegisterMCAsmInfoFn`:

* `createH2BLBMCAsmInfo` -> arch-specific syntax rules; currently selects ELF/Mach-O subclass.
* `createH2BLBMCSubtargetInfo` -> describes CPU features and subtarget variants (feature bits from `-mcpu`, `-mattr`).
* `createH2BLBMCInstrInfo` -> provides the table of machine instructions and their encodings/semantics.
* `createH2BLBMCRegisterInfo` -> defines the register set, aliases, and special registers (stack pointer, frame pointer, etc.).
* `createH2BLBMCInstPrinter` -> pretty-prints instructions from internal MC form back to textual assembly.
* `createH2BLBMCCodeEmitter` -> encodes MC instructions into raw binary machine code bytes.

---

## AsmParser

Parses assembly text into MC instructions for the target (used by `llvm-mc`, assembler, and disassembler).

### [H2BLBAsmParser.cpp](llvm/lib/Target/H2BLB/AsmParser/H2BLBAsmParser.cpp)

* `class H2BLBAsmParser : public MCTargetAsmParser`
* `class H2BLBOperand : public MCParsedAsmOperand`

### [H2BLBInstPrinter{.h/.cpp}](llvm/lib/Target/H2BLB/MCTargetDesc/H2BLBInstPrinter.h) (H2BLBInstPrinter : public MCInstPrinter)

Prints MCInst instructions back to assembly text.

### [H2BLBMCCodeEmitter{.h/.cpp}](llvm/lib/Target/H2BLB/MCTargetDesc/H2BLBMCCodeEmitter.h) (H2BLBMCCodeEmitter : public MCCodeEmitter)

Encodes MCInst instructions into raw binary machine code.

## [GIsel](llvm/lib/Target/H2BLB/GISel)

```
LLVM IR
  ↓
Generic MIR (G_ ops) and Call Lowering
  ↓
(H2BLBMandatoryPreLegalizerCombiner)  // cleanup/canonicalization
  ↓
(H2BLBLegalizerInfo)                  // make ops legal (widen, narrow, lower)
  ↓
(H2BLBRegisterBankInfo)               // assign registers to banks
  ↓
(H2BLBInstructionSelector)            // select into real H2BLB instructions
  ↓
MachineInstrs (target-specific)
  ↓
Register allocation / scheduling / MC lowering
```

# H2BLB CodeGen `.td` Files (Quick Overview)

## [`H2BLB.td`](llvm/lib/Target/H2BLB/H2BLB.td)

* **Entry point**: declares the target (`def H2BLB : Target`).
* **Includes** other `.td` files.
* Ensures all H2BLB `.inc` files are generated.

## [`H2BLBInstrInfo.td`](llvm/lib/Target/H2BLB/H2BLBInstrInfo.td)

* Declares **all instructions**.
* Defines opcodes, operands, ISel patterns.
* Produces `H2BLBGenInstrInfo.inc`.

## [`H2BLBInstrFormats.td`](llvm/lib/Target/H2BLB/H2BLBInstrFormats.td)

* Defines **instruction formats** (e.g. R/I/J types).
* Encapsulates bit layouts (opcode, regs, immediates).
* Reused by `InstrInfo.td`.

## [`H2BLBRegisterInfo.td`](llvm/lib/Target/H2BLB/H2BLBRegisterInfo.td)

* Declares **registers and classes**.
* Defines aliases and reserved regs.
* Produces `H2BLBGenRegisterInfo.inc`.

## [`H2BLBCallingConv.td`](llvm/lib/Target/H2BLB/H2BLBCallingConv.td)

* Declares **calling convention rules** for arguments and returns.
* Used by both SelectionDAG and GlobalISel call lowering.
* Produces `H2BLBGenCallingConv.inc`.

## [`H2BLBCombine.td`](llvm/lib/Target/H2BLB/H2BLBCombine.td)

* Declares **target-specific combine rules** for GlobalISel.
* Simplifies generic instructions (e.g. folding `add x, 0 → x`).
* Produces `H2BLBGenCombiner.inc`.

## [`H2BLBRegisterBankInfo.td`](llvm/lib/Target/H2BLB/GISel/H2BLBRegisterBankInfo.td)

* Declares **register banks** (e.g. GPR, FPR) for GlobalISel.
* Defines value mappings between virtual registers and banks.
* Produces `H2BLBGenRegisterBank.inc`.
