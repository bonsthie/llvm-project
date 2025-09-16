#include "H2BLB.h"
#include "TargetInfo/H2BLBTargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace llvm {
class H2BLBAsmPrinter : public AsmPrinter {
public:
  explicit H2BLBAsmPrinter(TargetMachine &TM,
                           std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "H2BLB Assembly Printer"; }

  void emitInstruction(const MachineInstr *MI) override;

private:
  MCInst machineInstrToMCInst(const MachineInstr &MI);
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCO);
};

} // namespace llvm

bool H2BLBAsmPrinter::lowerOperand(const MachineOperand &MO, MCOperand &MCO) {
  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    // ignore all implicit register operand
    if (MO.isImplicit())
      return false;
    MCO = MCOperand::createReg(MO.getReg());
    break;
  case MachineOperand::MO_RegisterMask:
    return false;
  case MachineOperand::MO_Immediate:
    MCO = MCOperand::createImm(MO.getImm());
    break;
  case MachineOperand::MO_MachineBasicBlock:
  case MachineOperand::MO_GlobalAddress:
  case MachineOperand::MO_ExternalSymbol:
  case MachineOperand::MO_MCSymbol:
  case MachineOperand::MO_JumpTableIndex:
  case MachineOperand::MO_ConstantPoolIndex:
  case MachineOperand::MO_BlockAddress:
    llvm_unreachable("not yet implemented");
  default:
    llvm_unreachable("unknow operand type");
  }
  return true;
}

MCInst H2BLBAsmPrinter::machineInstrToMCInst(const MachineInstr &MI) {
  MCInst TmpInst;
  TmpInst.setOpcode(MI.getOpcode());
  for (const MachineOperand &MO : MI.operands()) {
    MCOperand MCOp;
    if (lowerOperand(MO, MCOp))
      TmpInst.addOperand(MCOp);
  }
  return TmpInst;
}

void H2BLBAsmPrinter::emitInstruction(const MachineInstr *MI) {
  MCInst TmpInst = machineInstrToMCInst(*MI);
  EmitToStreamer(*OutStreamer, TmpInst);
}


// Register the AsmPrinter in the H2BLB target for other tools to find
// it (like Clang.)
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeH2BLBAsmPrinter() {
  RegisterAsmPrinter<H2BLBAsmPrinter> Tmp(getTheH2BLBTarget());
}
