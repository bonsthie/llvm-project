
#include "H2BLBRegisterInfo.h"
#include "MCTargetDesc/H2BLBMCTargetDesc.h" // For the enum of the regclasses.
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

#define GET_REGINFO_TARGET_DESC
#include "H2BLBGenRegisterInfo.inc"

using namespace llvm;

H2BLBRegisterInfo::H2BLBRegisterInfo() : H2BLBGenRegisterInfo(H2BLB::R0) {}

const MCPhysReg *H2BLBRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
	return nullptr;
}


bool H2BLBRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                         unsigned FIOperandNum,
                         RegScavenger *RS) const {
	return false;
}

BitVector H2BLBRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  return Reserved;
}

Register H2BLBRegisterInfo::getFrameRegister(const MachineFunction &FM) const {
	return {};
}

