#ifndef LLVM_LIB_TARGET_H2BLB_H2BLB_H
#define LLVM_LIB_TARGET_H2BLB_H2BLB_H
#include "llvm/IR/PassManager.h" // For PassInfoMixin.
#include "llvm/PassRegistry.h"

namespace llvm {
class Function;
class FunctionLoweringInfo;
class FastISel;
class InstructionSelector;
class H2BLBRegisterBankInfo;
class H2BLBSubtarget;
class H2BLBTargetMachine;
class Pass;
class PassRegistry;
class TargetLibraryInfo;

class H2BLBSimpleConstantPropagationNewPass
    : public llvm::PassInfoMixin<H2BLBSimpleConstantPropagationNewPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

void initializeH2BLBSimpleConstantPropagationPass(PassRegistry &);
Pass *createH2BLBSimpleConstantPropagationPassForLegacyPM();

void initializeH2BLBDAGToDAGISelLegacyPass(PassRegistry &);
Pass *createH2BLBISelDAG(H2BLBTargetMachine &TM);

void initializeH2BLBMandatoryPreLegalizerCombinerPass(PassRegistry &);
Pass *createH2BLBMandatoryPreLegalizerCombiner();

void initializeH2BLBMandatoryPostLegalizerCombinerPass(PassRegistry &);
Pass *createH2BLBMandatoryPostLegalizerCombiner();

namespace H2BLB {
FastISel *createFastISel(FunctionLoweringInfo &FuncInfo,
                         const TargetLibraryInfo *LibInfo);

InstructionSelector *createInstructionSelector(const H2BLBTargetMachine &,
                                               const H2BLBSubtarget &,
                                               const H2BLBRegisterBankInfo &);

} // end namespace H2BLB.
} // end namespace llvm.
#endif
