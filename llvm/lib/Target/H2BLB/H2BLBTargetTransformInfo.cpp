#include "H2BLBTargetTransformInfo.h"

using namespace llvm;

unsigned H2BLBTTIImpl::getLoadVectorFactor(unsigned VF, unsigned LoadSize,
                                           unsigned ChainSizeBytes,
                                           VectorType *VecTy) const {
  unsigned ElemSize = VecTy->getScalarSizeInBits();

  if (ElemSize != 16)
    return 0;
  return std::min(VF, 2u);
}
