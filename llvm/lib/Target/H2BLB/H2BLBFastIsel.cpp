#include "H2BLB.h"
#include "MCTargetDesc/H2BLBMCTargetDesc.h"
#include "H2BLBInstrInfo.h"
#include "MCTargetDesc/H2BLBMCTargetDesc.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/FastISel.h"
#include "llvm/CodeGen/FunctionLoweringInfo.h"

// this part has been abandoned 
// working on gisel only
using namespace llvm;

namespace llvm {
class H2BLBFastISel : public FastISel {
public:
  explicit H2BLBFastISel(FunctionLoweringInfo &FuncInfo,
                         const TargetLibraryInfo *LibInfo)
      : FastISel(FuncInfo, LibInfo, /* SkipTargetIndependentIsel=*/true) {}

  bool fastSelectInstruction(const Instruction *I) override;

// #include "H2BLBGenFastISel.inc"

private:
  bool selectRet(const Instruction &I);
};

} // namespace llvm

bool H2BLBFastISel::fastSelectInstruction(const Instruction *I) {
  // Skip FastISel is we've been told so
  if (TLI.fallBackToDAGISel(*I))
    return false;

  switch (I->getOpcode()) {
  default:
    break;
  case Instruction::Ret:
    return selectRet(*I);
  }

  return selectOperator(I, I->getOpcode());
}

bool H2BLBFastISel::selectRet(const Instruction &I) {
  if (FuncInfo.CanLowerReturn)
    return false;

  const Function &F = *I.getParent()->getParent();

  // GiveUp on anything fancy
  if (F.isVarArg())
    return false;
  if (TLI.supportSplitCSR(FuncInfo.MF))
    return false;

  const ReturnInst &Ret = cast<ReturnInst>(I);
  if (Ret.getNumOperands() > 0)
    return false;

  BuildMI(*FuncInfo.MBB, FuncInfo.InsertPt, MIMD, TII.get(H2BLB::RETURN));
  return true;
}

FastISel *H2BLB::createFastISel(FunctionLoweringInfo &FuncInfo,
                         const TargetLibraryInfo *LibInfo) {
  return new H2BLBFastISel(FuncInfo, LibInfo);
}
