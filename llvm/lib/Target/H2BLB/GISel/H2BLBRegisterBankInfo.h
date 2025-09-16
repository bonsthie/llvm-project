#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBREGISTERBANK_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBREGISTERBANK_H

#include "llvm/CodeGen/RegisterBankInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGBANK_DECLARATIONS
#include "H2BLBGenRegisterBank.inc"

namespace llvm {
class TargetRegisterInfo;

class H2BLBGenRegisterBankInfo : public RegisterBankInfo {
protected:
#define GET_TARGET_REGBANK_CLASS
#include "H2BLBGenRegisterBank.inc"
};

class H2BLBRegisterBankInfo final : public H2BLBGenRegisterBankInfo {
public:
  H2BLBRegisterBankInfo(const TargetRegisterInfo &TRI);

  const InstructionMapping &
  getInstrMapping(const MachineInstr &MI) const override;

  const RegisterBank &getRegBankFromRegClass(const TargetRegisterClass &RC,
                                             LLT Ty) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBREGISTERBANK_H
