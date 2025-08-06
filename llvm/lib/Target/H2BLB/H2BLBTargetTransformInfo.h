#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBTARGETTRANSFORMINFO_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBTARGETTRANSFORMINFO_H

#include "H2BLBISelLowering.h"
#include "H2BLBSubtarget.h"
#include "H2BLBTargetMachine.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/BasicTTIImpl.h"

namespace llvm {

// implementaion of the TargetTransformInfo class, from the Analysis library, is
// used to get some information about the cost of lowering LLVM IR constructs.
class H2BLBTTIImpl : public llvm::BasicTTIImplBase<H2BLBTTIImpl> {
  using BaseT = BasicTTIImplBase<H2BLBTTIImpl>;
  using TTI = TargetTransformInfo;

  friend BaseT;

  const H2BLBSubtarget &ST;
  const H2BLBTargetLowering &TLI;

  const H2BLBSubtarget *getST() const { return &ST; }
  const H2BLBTargetLowering *getTLI() const { return &TLI; }

public:
  explicit H2BLBTTIImpl(const H2BLBTargetMachine *TM, const Function &F)
      : BaseT(TM, F.getDataLayout()), ST(*TM->getSubtargetImpl(F)),
        TLI(*ST.getTargetLowering()) {}

  // those func are use by the optimiser to now how to optimise
  // there is a lot of them to override but askip that not the best way of doing
  // it
  unsigned getLoadVectorFactor(unsigned VF, unsigned LoadSize,
                               unsigned ChainSizeBytes,
                               VectorType *VecTy) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBTARGETTRANSFORMINFO_H
