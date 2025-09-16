#include "H2BLBSubtarget.h"
#include "GISel/H2BLBCallLowering.h"
#include "GISel/H2BLBLeglizerInfo.h"
#include "GISel/H2BLBRegisterBankInfo.h"
#include "H2BLBFrameLowering.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;

#define DEBUG_TYPE "h2blb-subtarget"

H2BLBSubtarget::H2BLBSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                               const TargetMachine &TM)
    : TargetSubtargetInfo(TT, CPU, /*TuneCPU=*/"", FS, /*PN=*/{}, /*PF=*/{},
                          /*PD=*/{},
                          /*WPR=*/nullptr,
                          /*WL=*/nullptr,
                          /*RA=*/nullptr, /*IS=*/nullptr,
                          /*OC=*/nullptr, /*FP=*/nullptr),
      TLInfo(TM, *this), FrameLowering(*this) {

  CallLoweringInfo.reset(new H2BLBCallLowering(*getTargetLowering()));
	Legalizer.reset(new H2BLBLegalizerInfo(*this));
	RegBankInfo.reset(new H2BLBRegisterBankInfo(*getRegisterInfo()));
}

// Pin the vtable to this file.
void H2BLBSubtarget::anchor() {}
