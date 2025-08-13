
#include "H2BLBFrameLowering.h"

using namespace llvm;

void H2BLBFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}

void H2BLBFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}

bool H2BLBFrameLowering::hasFPImpl(const MachineFunction &MF) const {
  return false;
}
