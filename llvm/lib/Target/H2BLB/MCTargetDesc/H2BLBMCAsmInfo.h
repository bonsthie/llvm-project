#ifndef LLVM_LIB_TARGET_H2BLB_H2BLB_MCTARGETDESC_H2BLBMASMINFO_H
#define LLVM_LIB_TARGET_H2BLB_H2BLB_MCTARGETDESC_H2BLBMASMINFO_H

#include "llvm/MC/MCAsmInfoDarwin.h"
#include "llvm/MC/MCAsmInfoELF.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/TargetParser/Triple.h"

namespace llvm {

class H2BLBMCAsmInfoELF : public MCAsmInfoELF {
public :
		explicit H2BLBMCAsmInfoELF(const Triple &TT, const MCTargetOptions &Options);
};

class H2BLBMCAsmInfoDarwin : public MCAsmInfoDarwin {
public :
		explicit H2BLBMCAsmInfoDarwin(const Triple &TT, const MCTargetOptions &Options);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLB_MCTARGETDESC_H2BLBMASMINFO_H
