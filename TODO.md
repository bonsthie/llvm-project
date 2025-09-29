# GlobalISel X86 TODO

Detailed follow-up items to make the X86 GlobalISel pipeline feature-complete. Each section lists concrete tasks plus an example that currently fails or falls back.

## Call Lowering & ABI Coverage
- Implement variadic lowering in `X86CallLowering` so SysV varargs stay in GISel.
  - *Example:* `llvm/test/CodeGen/X86/GlobalISel/callingconv.ll` lacks coverage for varargs; compile with `-global-isel-abort=1` to see SelectionDAG fallback.
- Support byval/inreg/SwiftSelf/SwiftError/Nest attributes and multi-register aggregates during argument splitting.
  - *Example:* `llvm/test/CodeGen/X86/GlobalISel/x86_64-fallback.ll` falls back on a byval call.
- Add calling-convention lowering for Win64, stdcall/fastcall, vectorcall, and C++ member functions.
  - *Example:* drive with `llc -mtriple=x86_64-windows-msvc -global-isel` on a simple Windows ABI test; currently rejected in `X86CallLowering.cpp` around lines 328-331.
- Handle tail calls, sret demotion, and multi-register returns without aborting.
  - *Example:* Extend `llvm/test/CodeGen/X86/GlobalISel/calllowering-tailcall.ll` to assert no fallback with `-global-isel-abort=1`.

## Address Materialisation & Globals
- Teach `X86InstructionSelector::selectGlobalValue` and `materializeFP` to cope with TLS (both local-dynamic and initial-exec).
  - *Example:* a TLS access like `@__tls_get_addr` currently hits the TODO at `X86InstructionSelector.cpp:585`.
- Support PIC-base-relative stubs and GOT references when classifying globals.
  - *Example:* small Linux PIC builds with `-fPIC` fail because `isGlobalRelativeToPICBase` returns true at `X86InstructionSelector.cpp:597`.
- Implement large code-model constant-pool materialisation (use two-step loads or MOVABS).
  - *Example:* forcing `-mcmodel=large` trips the TODO at `X86InstructionSelector.cpp:610`.

## Atomics & Memory Operations
- Extend load/store selection to accept atomic orderings beyond `unordered` and handle unaligned atomics when ISA allows.
  - *Example:* `G_LOAD` with `seq_cst` currently prints "Atomic ordering not supported yet" at `X86InstructionSelector.cpp:650`.
- Legalise and select `G_ATOMIC_*` (RMW, fence, cmpxchg) using the lock-prefixed X86 instructions.
  - *Example:* add MIR tests under `llvm/test/CodeGen/X86/GlobalISel/atomics/` to assert the selector emits `LOCK XADD`.
- Wire up 128-bit integer operations (mul/div/rem) in the legaliser and selector.
  - *Example:* `llvm/test/CodeGen/X86/GlobalISel/mul-scalar.ll` still comments out the i128 case because selection fails.

## Vector & Floating-Point Support
- Allow `G_INSERT`/`G_EXTRACT` to work for scalars as well as vectors; fill in subregister handling.
  - *Example:* enabling `roundeven_f16` hits "TODO: support scalar types" in `X86InstructionSelector.cpp:1366`.
- Finish legalization for `G_BUILD_VECTOR` / `G_INSERT_VECTOR_ELT` so cvtt* patterns can be selected without falling back.
  - *Example:* `llvm/test/CodeGen/X86/GlobalISel/roundeven.ll` comments out the vector case due to build_vector legalization gaps.
- Expand register-bank intrinsic coverage beyond the small SSE1 list.
  - *Example:* adding an AVX intrinsic like `_mm_dp_ps` trips the TODO in `X86RegisterBankInfo.cpp:50`.
- Fix FP constant materialisation for 32-bit PIC by allocating and using the PIC base register.
  - *Example:* `materializeFP` returns false for `MO_GOTOFF` at `X86InstructionSelector.cpp:1600-1602`.

## TableGen Automation & Pattern Coverage
- Move hand-written selection code into TableGen patterns once legalization allows (loads/stores, extensions, arithmetic with immediates).
  - *Example:* `getLoadStoreOp` and `selectConstant` are flagged for TableGen removal at `X86InstructionSelector.cpp:73-78`.
- Generate pattern coverage for condition-code emitting ops (cmp, setcc, select) to get rid of ad-hoc `BuildMI` sequences.
  - *Example:* `selectCmp`/`selectFCmp` still emit multiple instructions manually; new `GIMatchTable` entries could replace them.
- Backfill MIR tests under `llvm/test/CodeGen/X86/GlobalISel` whenever a fallback TODO is resolved; keep `x86_64-fallback.ll` empty by design once complete.
  - *Example:* after handling byval, update `x86_64-fallback.ll` to ensure no remarks remain.
