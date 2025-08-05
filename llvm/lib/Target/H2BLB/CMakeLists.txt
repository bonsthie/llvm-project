add_llvm_component_group(H2BLB)

add_llvm_target(H2BLBCodeGen
	H2BLBTargetMachine.cpp
	H2BLBISelLowering.cpp
	H2BLBSubtarget.cpp

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
