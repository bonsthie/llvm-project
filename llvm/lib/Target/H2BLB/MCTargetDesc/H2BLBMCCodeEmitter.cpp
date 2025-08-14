
#include "H2BLBMCCodeEmitter.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/EndianStream.h"
#include <cstdint>

#define DEBUG_TYPE "mccodeemitter"

#define GET_REGINFO_ENUM
#include "H2BLBGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "H2BLBGenInstrInfo.inc"

using namespace llvm;

unsigned
H2BLBMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return MCCtxt.getRegisterInfo()->getEncodingValue(MO.getReg());
  assert(MO.isImm() && "Unsupported operand type");
  return static_cast<unsigned>(MO.getImm());
}

void H2BLBMCCodeEmitter::encodeInstruction(const MCInst &MI,
                                           SmallVectorImpl<char> &CB,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  // Get instruction encoding and emit it
  uint64_t Encoding = getBinaryCodeForInstr(MI, Fixups, STI);
  assert(((Encoding & 0xffffffffffff0000) == 0) &&
         "Only the first 16-bit should be set");
  support::endian::write<uint16_t>(CB, Encoding, llvm::endianness::little);
}

#include "H2BLBGenMCCodeEmitter.inc"
