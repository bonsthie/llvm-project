

#include "H2BLB.h"
#include "H2BLBISelLowering.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/FastISel.h"
#include "llvm/CodeGen/FunctionLoweringInfo.h"

using namespace llvm;

H2BLBTargetLowering::H2BLBTargetLowering(const TargetMachine &TM)
    : TargetLowering(TM) {}

FastISel *
H2BLBTargetLowering::createFastISel(FunctionLoweringInfo &FuncInfo,
                                    const TargetLibraryInfo *LibInfo) const {
	return H2BLB::createFastISel(FuncInfo, LibInfo);
}
