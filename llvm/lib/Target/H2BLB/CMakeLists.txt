add_llvm_component_group(H2BLB)

set(LLVM_TARGET_DEFINITIONS H2BLB.td)

tablegen(LLVM H2BLBGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM H2BLBGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM H2BLBGenInstrInfo.inc -gen-instr-info)

# MCInstr to .s
tablegen(LLVM H2BLBGenAsmWriter.inc -gen-asm-writer)
# .s to MCInst
tablegen(LLVM H2BLBGenAsmMatcher.inc -gen-asm-matcher)
# Encoding
tablegen(LLVM H2BLBGenMCCodeEmitter.inc -gen-emitter)

# ISel 
tablegen(LLVM H2BLBGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM H2BLBGenFastISel.inc -gen-fast-isel)

tablegen(LLVM H2BLBGenCallingConv.inc -gen-callingconv)

tablegen(LLVM H2BLBGenRegisterBank.inc -gen-register-bank)

tablegen(LLVM H2BLBGenGlobalISel.inc -gen-global-isel)


add_public_tablegen_target(H2BLBCommonTableGen)

add_llvm_target(H2BLBCodeGen
	H2BLBConstantPropagationPass.cpp
	H2BLBISelLowering.cpp
	H2BLBSubtarget.cpp
	H2BLBTargetMachine.cpp
	H2BLBTargetTransformInfo.cpp
	H2BLBTargetTransformInfo.cpp
	H2BLBTargetObjectFile.cpp
	H2BLBRegisterInfo.cpp
	H2BLBInstrInfo.cpp
	H2BLBFrameLowering.cpp
	H2BLBAsmPrinter.cpp
	H2BLBIselDagToDag.cpp
	H2BLBFastIsel.cpp
	H2BLBCallingConv.cpp

	GISel/H2BLBCallLowering.cpp
	GISel/H2BLBLeglizerInfo.cpp
	GISel/H2BLBRegisterBankInfo.cpp

# target depends on the listed libraries
	LINK_COMPONENTS
	H2BLBDesc
	H2BLBInfo
	H2BLBAsmParser

    Analysis
	AsmPrinter
    CodeGen
	CodeGenTypes
    Core
	GlobalISel
    MC
    Passes
    SelectionDAG
    Support
    Target
    TargetParser
    TransformUtils

# makes sure this target is added to the LLVM created by add_llvm_component_group
	ADD_TO_COMPONENT
	H2BLB
)

add_subdirectory(MCTargetDesc)
add_subdirectory(AsmParser)
add_subdirectory(TargetInfo)
