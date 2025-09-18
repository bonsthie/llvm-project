#include "H2BLB.h"
#include "H2BLBTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/Pass.h"
#include <memory>

#define DEBUG_TYPE "h2blb-isel"
#define PASS_NAME "H2BLB DAG->DAG Pattern Instruction Selection"

namespace llvm {

class H2BLBDAGToDAGIsel : public SelectionDAGISel {
public:
  explicit H2BLBDAGToDAGIsel(TargetMachine &TM) : SelectionDAGISel(TM) {}

private:
  void Select(SDNode *N) override;

// gen SelectCode
// #include "H2BLBGenDAGISel.inc"
};

class H2BLBDAGToDAGISelLegacy : public SelectionDAGISelLegacy {
public:
  static char ID;
  H2BLBDAGToDAGISelLegacy(H2BLBTargetMachine &TM)
      : SelectionDAGISelLegacy(ID, std::make_unique<H2BLBDAGToDAGIsel>(TM)) {}
};

} // namespace llvm

using namespace llvm;

void H2BLBDAGToDAGIsel::Select(SDNode *N) {
  if (N->isMachineOpcode())
    return;
  // SelectCode(N);
}


char H2BLBDAGToDAGISelLegacy::ID = 0;

INITIALIZE_PASS(H2BLBDAGToDAGISelLegacy, DEBUG_TYPE,PASS_NAME, false, false)

Pass *llvm::createH2BLBISelDAG(H2BLBTargetMachine &TM) {
	return new H2BLBDAGToDAGISelLegacy(TM);
}
