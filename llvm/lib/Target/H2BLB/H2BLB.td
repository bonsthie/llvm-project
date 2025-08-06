

include "llvm/Target/Target.td"

def H2BLBInstInfo : InstrInfo;

def H2BLB : Target {
	let InstructionSet = H2BLBInstInfo;
}
