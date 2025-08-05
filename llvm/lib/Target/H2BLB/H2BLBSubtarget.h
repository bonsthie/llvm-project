#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H

#include "H2BLBISelLowering.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
namespace llvm {

// This class exposes several getter functions to access some of the low-
// level target APIs, such as the target-specific implementation of the
// TargetLowering class.
class H2BLBSubtarget : public TargetSubtargetInfo {
  H2BLBTargetLowering TLInfo;

public:
  H2BLBSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                 const TargetMachine &TM);

  const H2BLBTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }

	const TargetRegisterInfo *getRegisterInfo() const override { return nullptr; }

private:
  virtual void anchor();
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBSUBTARGET_H
