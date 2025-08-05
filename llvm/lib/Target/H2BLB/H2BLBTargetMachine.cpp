#include "H2BLBTargetMachine.h"
#include "TargetInfo/H2BLBTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"

// look at chapter 7 to understand this
static const char *H2BLBDataLayoutStr = "e-p:16:32:32-n16:32-i1:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-v32:32:32";

H2BLBTargetMachine::H2BLBTargetMachine(const Target &T,  //
                                       const Triple &TT, //
                                       StringRef CPU,    //
                                       StringRef FS,     //
                                       const TargetOptions &Options,
                                       std::optional<Reloc::Model> RM,
                                       std::optional<CodeModel::Model> CM, //
                                       CodeGenOptLevel OL,                 //
                                       bool JIT)
    : CodeGenTargetMachineImpl(T, H2BLBDataLayoutStr, TT, CPU, FS, Options,
                               RM ? *RM : Reloc::Static,
                               CM ? *CM : CodeModel::Small, OL) {}

H2BLBTargetMachine::~H2BLBTargetMachine() = default;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeH2BLBTarget() {
  RegisterTargetMachine<H2BLBTargetMachine> X(llvm::getTheH2BLBTarget());
}
