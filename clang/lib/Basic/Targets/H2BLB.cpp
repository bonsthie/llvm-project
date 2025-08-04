#include "H2BLB.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/ADT/SmallVector.h"

using namespace clang;
using namespace clang::targets;

void H2BLBTargetInfo::getTargetDefines(const LangOptions &Opts, MacroBuilder &Builder) const {
  Builder.defineMacro("__H2BLB__", "1");
}

llvm::SmallVector<Builtin::InfosShard>
H2BLBTargetInfo::getTargetBuiltins() const {
  return llvm::SmallVector<Builtin::InfosShard>(); // TODO H2BLB
}
