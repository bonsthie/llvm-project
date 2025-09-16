#ifndef LLVM_LIB_TARGET_H2BLB_GISEL_H2BLBMACHINELEGALIZER_H
#define LLVM_LIB_TARGET_H2BLB_GISEL_H2BLBMACHINELEGALIZER_H

#include "H2BLBSubtarget.h"
#include "llvm/CodeGen/GlobalISel/GISelChangeObserver.h"
#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"
#include "llvm/CodeGen/GlobalISel/LostDebugLocObserver.h"
namespace llvm {
class MachineIRBuilder;
class GIselChangeObserver;

class H2BLBLegalizerInfo : public LegalizerInfo {
  const H2BLBSubtarget &ST;

public:
  H2BLBLegalizerInfo(const H2BLBSubtarget &ST);

  bool legalizeCustom(LegalizerHelper &Helper, MachineInstr &MI,
                      LostDebugLocObserver &LocObserver) const override;

private:
  bool legalizeMul(MachineInstr &MI, MachineRegisterInfo &MRI,
                   MachineIRBuilder &MIRBuilder,
                   GISelChangeObserver &Observer) const;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_GISEL_H2BLBMACHINELEGALIZER_H
