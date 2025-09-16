#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBFRAMELOWERING_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBFRAMELOWERING_H

#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class H2BLBSubtarget;

class H2BLBFrameLowering : public TargetFrameLowering {
public:
  explicit H2BLBFrameLowering(const H2BLBSubtarget &)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(8), 0) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

protected:
  bool hasFPImpl(const MachineFunction &MF) const override;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBFRAMELOWERING_H
