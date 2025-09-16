include "H2BLBInstrFormats.td"

// SelectionDag specific node
// definition of the call instruction that take a ptr to the callee
def H2BLBcall : SDNode<"H2BLBISD::CALL",
                       // Type profile:
                       // 0: no result.
                       // -1: a variadic number of inputs.
                       // The first operand must be a pointer (to the call
                       // target)
                       SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>,
                       [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                        SDNPVariadic]>;

def H2BLBreturnglue : SDNode<"H2BLBISD::RETURN_GLUE", SDTNone,
                             [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def H2BLBcallseq_start : SDNode<"ISD::CALLSEQ_START",
                                // Two arguments:
                                // Number of bytes allocated within the
                                // callseq_start/end region and the number
                                // of bytes allocated outside.
                                SDCallSeqStart<[ SDTCisVT<0, i32>,
                                                 SDTCisVT<1, i32> ]>,
                                [SDNPHasChain, SDNPOutGlue]>;
def H2BLBcallseq_end : SDNode<"ISD::CALLSEQ_END",
                              // Same arguments as callseq_start.
                              SDCallSeqEnd<[ SDTCisVT<0, i32>,
                                             SDTCisVT<1, i32> ]>,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def SDTypeWideningProfile : SDTypeProfile<1, 2, [SDTCisVT<0,i32>,
                                                 SDTCisVT<1,i16>,
                                                 SDTCisSameAs<1,2>]>;
def H2BLBwidening_smul : SDNode<"H2BLBISD::WIDENING_SMUL",
                                SDTypeWideningProfile, [SDNPCommutative]>;
def H2BLBwidening_umul : SDNode<"H2BLBISD::WIDENING_UMUL",
                                SDTypeWideningProfile, [SDNPCommutative]>;


let Pattern = [(set GPR16:$dst, (add GPR16:$src0, GPR16:$src1))] in
def ADDi16rr : H2BLBBinaryInstruction<"addi16",
                                      /*opcode=*/0,
                                      /*is32Bit=*/false>;

let Pattern = [(set (i32 GPR32:$dst), (add (i32 GPR32:$src0),
                                           (i32 GPR32:$src1)))] in
def ADDi32rr : H2BLBBinaryInstruction<"addi32",
                                      /*opcode=*/0,
                                      /*is32Bit=*/true>;


let Pattern = [(set GPR32:$dst, (int_h2blb_widening_smul GPR16:$src0, GPR16:$src1))] in
def WIDENING_SMUL : H2BLBWidenIMul<"wsmul", /*isSign=*/1>;
let Pattern = [(set GPR32:$dst, (int_h2blb_widening_umul GPR16:$src0, GPR16:$src1))] in
def WIDENING_UMUL : H2BLBWidenIMul<"wumul", /*isSign=*/0>;

let Uses = [R0], isReturn = 1, isTerminator = 1, isBarrier = 1 in
def RETURN : H2BLBNoOperandInstruction<"ret", /*opcode=*/0b00001, /*is32Bit=*/0>;

// Pseudo return instruction to make the machine verifier happy during
// GlobalISel lowering. Indeed, the live-range of R0 will only be correct
// after we run FinalizeISel.
let isReturn = 1, isTerminator = 1, isBarrier = 1, usesCustomInserter = 1 in
def RET_PSEUDO : H2BLBPseudoInstruction<(outs), (ins), []>;

let isCall = 1, Defs = [R0], Uses = [SP] in
	def CALL : H2BLBInstruction<"call", "$tgt",
                                 /*opcode=*/0b00100,
                                 /*is32Bit=*/0,
                                 /*oops=*/(outs),
                                 /*iops=*/(ins i16imm:$tgt)> {
	bits<11> tgt;
	let Inst{10-0} = tgt;
	let Pattern = [(H2BLBcall tglobaladdr:$tgt)];
}


let hasSideEffects = 1, isCodeGenOnly = 1 in {
  let Defs = [SP], Uses = [SP] in {

    def ADJCALLSTACKDOWN : H2BLBPseudoInstruction<
        (outs), (ins i32imm:$amt1, i32imm:$amt2),
        []> {
      let AsmString = "";
      let isPseudo  = 1;
    }

    def ADJCALLSTACKUP : H2BLBPseudoInstruction<
        (outs), (ins i32imm:$amt1, i32imm:$amt2),
        []> {
      let AsmString = "";
      let isPseudo  = 1;
    }
  }
}


