#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBFRAMELOWERING_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class H2BLBFrameLowering : public TargetFrameLowering {

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFPImpl(const MachineFunction &MF) const override;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBFRAMELOWERING_H
