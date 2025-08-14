add_llvm_component_library(LLVMH2BLBDesc
	H2BLBMCTargetDesc.cpp
	H2BLBMAsmInfo.cpp
	H2BLBInstPrinter.cpp
	H2BLBMCCodeEmitter.cpp

	LINK_COMPONENTS
	MC
	Support
	H2BLBInfo

	ADD_TO_COMPONENT
	H2BLB
)
