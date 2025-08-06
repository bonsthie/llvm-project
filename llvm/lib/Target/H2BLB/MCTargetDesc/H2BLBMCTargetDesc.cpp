#include "H2BLBMCTargetDesc.h"
#include "H2BLBMCAsmInfo.h"
#include "TargetInfo/H2BLBTargetInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_SUBTARGETINFO_MC_DESC
#include "H2BLBGenSubtargetInfo.inc"

static MCSubtargetInfo *
createH2BLBMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createH2BLBMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCRegisterInfo *createH2BLBMCRegisterInfo(const Triple &Triple) {
  MCRegisterInfo *X = new MCRegisterInfo();
  return X;
}

static MCInstrInfo *createH2BLBMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  return X;
}

static MCAsmInfo *createH2BLBMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TheTriple,
                                       const MCTargetOptions &Options) {
  MCAsmInfo *MAI;
  if (TheTriple.isOSBinFormatMachO())
    MAI = new H2BLBMCAsmInfoDarwin(TheTriple, Options);
  else if (TheTriple.isOSBinFormatELF())
    MAI = new H2BLBMCAsmInfoELF(TheTriple, Options);
  else
    report_fatal_error("Binary format not supported");
  return MAI;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeH2BLBTargetMC() {
  Target &TheTarget = getTheH2BLBTarget();

  // Register the MC asm info.
  RegisterMCAsmInfoFn X(TheTarget, createH2BLBMCAsmInfo);

  TargetRegistry::RegisterMCSubtargetInfo(TheTarget,
                                          createH2BLBMCSubtargetInfo);

  TargetRegistry::RegisterMCInstrInfo(TheTarget, createH2BLBMCInstrInfo);

  TargetRegistry::RegisterMCRegInfo(TheTarget, createH2BLBMCRegisterInfo);
}
