#ifndef LLVM_LIB_TARGET_H2BLB_H2BLB_CALLINGCONV_H
#define LLVM_LIB_TARGET_H2BLB_H2BLB_CALLINGCONV_H

#include "llvm/CodeGen/CallingConvLower.h"

namespace llvm {
bool CC_H2BLB_Common(unsigned ValNo, MVT ValVT, MVT LocVT,
                     CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
                     CCState &State);
bool RetCC_H2BLB_Common(unsigned ValNo, MVT ValVT, MVT LocVT,
                        CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
                        CCState &State);
} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLB_CALLINGCONV_H
