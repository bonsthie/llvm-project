#include "H2BLB.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/MacroBuilder.h"
#include "clang/Basic/TargetBuiltins.h"
#include "llvm/ADT/SmallVector.h"

using namespace clang;
using namespace clang::targets;

void H2BLBTargetInfo::getTargetDefines(const LangOptions &Opts,
                                       MacroBuilder &Builder) const {
  Builder.defineMacro("__H2BLB__", "1");
}

namespace clang::targets::H2BLB {
#define GET_BUILTIN_STR_TABLE
#include "clang/Basic/BuiltinsH2BLB.inc"
#undef GET_BUILTIN_STR_TABLE

static constexpr Builtin::Info BuiltinInfos[] = {
#define GET_BUILTIN_INFOS
#include "clang/Basic/BuiltinsH2BLB.inc"
#undef GET_BUILTIN_INFOS
};

// static constexpr Builtin::Info PrefixedBuiltinInfos[] = {
// #define GET_BUILTIN_PREFIXED_INFOS
// #include "clang/Basic/BuiltinsH2BLB.inc"
// #undef GET_BUILTIN_PREFIXED_INFOS
// };
} // namespace clang::targets::H2BLB

llvm::SmallVector<Builtin::InfosShard>
H2BLBTargetInfo::getTargetBuiltins() const {
  return {
    { &H2BLB::BuiltinStrings, H2BLB::BuiltinInfos },
  };
}
