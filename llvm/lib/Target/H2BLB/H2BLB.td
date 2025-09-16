// Include the generic target definitions.
include "llvm/Target/Target.td"

include "H2BLBRegisterInfo.td"
include "H2BLBCallingConv.td"
include "H2BLBInstrInfo.td"
include "GISel/H2BLBRegisterBankInfo.td"

def H2BLBInstrInfo : InstrInfo;

def H2BLBAsmParser : AsmParser;

def H2BLB : Target {
  let InstructionSet = H2BLBInstrInfo;
  let AssemblyParsers = [H2BLBAsmParser];
}
