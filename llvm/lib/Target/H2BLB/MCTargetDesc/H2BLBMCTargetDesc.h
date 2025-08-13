#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBMCTARGETDESC_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBMCTARGETDESC_H

#include <cstdint> // For int16_t and so on used in the .inc files.

namespace llvm {


} // namespace llvm

#define GET_REGINFO_ENUM
#include "H2BLBGenSubtargetInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "H2BLBGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_MC_HELPER_DECLS
#include "H2BLBGenInstrInfo.inc"


#endif //  LLVM_LIB_TARGET_H2BLB_H2BLBMCTARGETDESC_H
