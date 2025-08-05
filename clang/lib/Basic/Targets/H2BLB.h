#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_H2BLB_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_H2BLB_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/Support/Compiler.h" // For LLVM_LIBRARY_VISIBILITY.
#include "llvm/TargetParser/Triple.h"

namespace clang {
namespace targets {

// H2BLB class
class LLVM_LIBRARY_VISIBILITY H2BLBTargetInfo : public TargetInfo {
public:
  H2BLBTargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    resetDataLayout( "e-p:16:32:32-n16:32-i1:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-v32:32:32");
  }

  /// Appends the target-specific \#define values for this
  /// target set to the specified buffer.
  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  /// Return information about target-specific builtins for
  /// the current primary target, and info about which builtins are non-portable
  /// across the current set of primary and secondary targets.
  llvm::SmallVector<Builtin::InfosShard> getTargetBuiltins() const override;

  /// Returns the kind of __builtin_va_list type that should be used
  /// with this target.
  BuiltinVaListKind getBuiltinVaListKind() const override {
    return CharPtrBuiltinVaList;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &info) const override {
    return false;
  }

  /// Returns a string of target-specific clobbers, in LLVM format.
  std::string_view getClobbers() const override { return ""; }

  ArrayRef<const char *> getGCCRegNames() const override {
    return ArrayRef<const char *>();
  }
  ArrayRef<GCCRegAlias> getGCCRegAliases() const override {
    return ArrayRef<GCCRegAlias>();
  }
};
} // namespace targets
} // namespace clang
#endif // LLVM_CLANG_LIB_BASIC_TARGETS_H2BLB_H
