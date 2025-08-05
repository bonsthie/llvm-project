#include "H2BLBSubtarget.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;

// Pin the vtable to this file.
void H2BLBSubtarget::anchor() {}

H2BLBSubtarget::H2BLBSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                               const TargetMachine &TM)
    : TargetSubtargetInfo(TT, CPU, /*TuneCPU=*/"", FS, /*PN=*/{}, /*PF=*/{},
                          /*PD=*/{},
                          /*WPR=*/nullptr,
                          /*WL=*/nullptr,
                          /*RA=*/nullptr, /*IS=*/nullptr,
                          /*OC=*/nullptr, /*FP=*/nullptr),
      TLInfo(TM) {}
