#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H

#include "H2BLBISelLowering.h"
#include "H2BLBFrameLowering.h"
#include "H2BLBInstrInfo.h"
#include "H2BLBRegisterInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/GlobalISel/CallLowering.h"
#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"
#include "llvm/CodeGen/RegisterBankInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "H2BLBGenSubtargetInfo.inc"

namespace llvm {

// This class exposes several getter functions to access some of the low-
// level target APIs, such as the target-specific implementation of the
// TargetLowering class.
class H2BLBSubtarget : public TargetSubtargetInfo {
  H2BLBTargetLowering TLInfo;
  H2BLBRegisterInfo RegisterInfo;
  H2BLBInstrInfo InstrInfo;
  H2BLBFrameLowering FrameLowering;
  SelectionDAGTargetInfo SDTgtInfo;

  /// GIsel realted apis
  std::unique_ptr<CallLowering> CallLoweringInfo;
  std::unique_ptr<LegalizerInfo> Legalizer;
  std::unique_ptr<RegisterBankInfo> RegBankInfo;

public:
  H2BLBSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                 const TargetMachine &TM);

  const H2BLBTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }

  const TargetRegisterInfo *getRegisterInfo() const override {
    return &RegisterInfo;
  }

  const TargetInstrInfo *getInstrInfo() const override { //
    return &InstrInfo;
  }

  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }

  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &SDTgtInfo;
  }

  const CallLowering *getCallLowering() const override {
    return CallLoweringInfo.get();
  }

  const LegalizerInfo *getLegalizerInfo() const override {
    return Legalizer.get();
  }

  const RegisterBankInfo *getRegBankInfo() const override {
    return RegBankInfo.get();
  }

private:
  virtual void anchor();
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H
