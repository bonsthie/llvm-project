#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBREGISTERINFO_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "H2BLBFrameLowering.h"

#define GET_REGINFO_HEADER
#include "H2BLBGenRegisterInfo.inc"

namespace llvm {
struct H2BLBRegisterInfo : public llvm::H2BLBGenRegisterInfo {
  H2BLBRegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &FM) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBREGISTERINFO_H
