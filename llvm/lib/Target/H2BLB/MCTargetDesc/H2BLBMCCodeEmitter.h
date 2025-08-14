#ifndef LLVM_LIB_TARGET_H2BLB_H2BLB_MCTARGETDESC_H2BLBMCCODEEMITTER_H
#define LLVM_LIB_TARGET_H2BLB_H2BLB_MCTARGETDESC_H2BLBMCCODEEMITTER_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include <cstdint>
#include <stdint.h>

namespace llvm {
class H2BLBMCCodeEmitter : public MCCodeEmitter {
  MCContext &MCCtxt;

public:
  H2BLBMCCodeEmitter(MCContext &MCCtxt) : MCCodeEmitter(), MCCtxt(MCCtxt) {}
  ~H2BLBMCCodeEmitter() override = default;

  // TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  // Return binary encoding of operand. If the machine
  // operand requires relocation, record the relocation and return zero.
  // This method is used in the TableGen'erated code.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  void encodeInstruction(const MCInst &MI, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLB_MCTARGETDESC_H2BLBMCCODEEMITTER_H
