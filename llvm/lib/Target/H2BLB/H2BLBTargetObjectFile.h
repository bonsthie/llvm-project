#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBTARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBTARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {

class H2BLB_ELFTargetObjectFile : public llvm::TargetLoweringObjectFileELF {
public :
	H2BLB_ELFTargetObjectFile();

};


class H2BLB_MachoOTargetObjectFile : public llvm::TargetLoweringObjectFileMachO {
public :
	H2BLB_MachoOTargetObjectFile();

};

}

#endif //LLVM_LIB_TARGET_H2BLB_H2BLBTARGETOBJECTFILE_H
