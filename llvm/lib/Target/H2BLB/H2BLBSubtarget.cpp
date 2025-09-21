#include "H2BLBSubtarget.h"
#include "GISel/H2BLBCallLowering.h"
#include "GISel/H2BLBLeglizerInfo.h"
#include "GISel/H2BLBRegisterBankInfo.h"
#include "H2BLB.h"
#include "H2BLBFrameLowering.h"
#include "H2BLBTargetMachine.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;

#define DEBUG_TYPE "h2blb-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "H2BLBGenSubtargetInfo.inc"

H2BLBSubtarget::H2BLBSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                               const TargetMachine &TM)
    : H2BLBGenSubtargetInfo(TT, CPU, /*TuneCPU=*/"", FS),
      TLInfo(TM, *this), FrameLowering(*this) {

  CallLoweringInfo.reset(new H2BLBCallLowering(*getTargetLowering()));
  Legalizer.reset(new H2BLBLegalizerInfo(*this));

  auto *RBI = new H2BLBRegisterBankInfo(*getRegisterInfo());
  InstrSelector.reset(H2BLB::createInstructionSelector(
      *static_cast<const H2BLBTargetMachine *>(&TM), *this, *RBI));

  RegBankInfo.reset(RBI);
}

// Pin the vtable to this file.
void H2BLBSubtarget::anchor() {}
