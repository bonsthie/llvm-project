include "llvm/Target/Target.td"

include "H2BLBRegisterInfo.td"
include "H2BLBInstrInfo.td"

def H2BLBInstInfo : InstrInfo;

def H2BLB : Target {
	let InstructionSet = H2BLBInstInfo;
}
