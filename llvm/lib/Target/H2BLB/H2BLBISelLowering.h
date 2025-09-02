#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBISELLOWERING_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBISELLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

// This class implements the logic to lower LLVM IR to the legacy instruction
// selector, SelectionDAG.
class H2BLBTargetLowering : public TargetLowering {
public:
  explicit H2BLBTargetLowering(const TargetMachine &TM);

  FastISel *createFastISel(FunctionLoweringInfo &FuncInfo,
                           const TargetLibraryInfo *LibInfo) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBISELLOWERING_H
