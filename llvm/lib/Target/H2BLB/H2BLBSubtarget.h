#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H

#include "H2BLBFrameLowering.h"
#include "H2BLBISelLowering.h"
#include "H2BLBInstrInfo.h"
#include "H2BLBRegisterInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
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

private:
  virtual void anchor();
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H
