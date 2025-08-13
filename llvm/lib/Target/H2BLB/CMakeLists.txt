add_llvm_component_group(H2BLB)

set(LLVM_TARGET_DEFINITIONS H2BLB.td)

tablegen(LLVM H2BLBGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM H2BLBGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM H2BLBGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM H2BLBInstrFormats.inc -gen-emitter)


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

# target depends on the listed libraries
	LINK_COMPONENTS
	H2BLBDesc
	H2BLBInfo
    Analysis
    CodeGen
    Core
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
add_subdirectory(TargetInfo)
