#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBTARGETMACHINE_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBTARGETMACHINE_H

#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/Target/TargetOptions.h"
#include <optional>

using namespace llvm;


class H2BLBTargetMachine : public CodeGenTargetMachineImpl {
public:
  H2BLBTargetMachine(const Target &T,  //
                     const Triple &TT, //
                     StringRef CPU,    // cpu name
                     StringRef FS,     // feature string (ex: -Xclang -target-feature -Xclang +sse2,-sse)
                     const TargetOptions &Options, // sets default behavior for target (ex: how math isnt are interpreted)
                     std::optional<Reloc::Model> RM, // relocation model (how symbols are accessed)
                     std::optional<CodeModel::Model> CM, // codegen model (ex : size settings of the binary)
                     CodeGenOptLevel OL,                 // optimisation level
                     bool JIT); // has jit
	~H2BLBTargetMachine() override;
};

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBTARGETMACHINE_H
