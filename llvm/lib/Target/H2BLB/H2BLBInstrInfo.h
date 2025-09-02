
#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBINSTRINFO_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBINSTRINFO_H

#include "MCTargetDesc/H2BLBMCTargetDesc.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "H2BLBGenInstrInfo.inc"

namespace llvm {

class H2BLBInstrInfo : public H2BLBGenInstrInfo {
public:
  H2BLBInstrInfo();
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBINSTRINFO_H
