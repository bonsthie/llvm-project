#ifndef LLVM_LIB_TARGET_H2BLB_H2BLBTARGETMACHINE_H
#define LLVM_LIB_TARGET_H2BLB_H2BLBTARGETMACHINE_H

#include "H2BLBSubtarget.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Target/TargetOptions.h"
#include <optional>

namespace llvm {

class H2BLBTargetMachine : public CodeGenTargetMachineImpl {
  mutable std::unique_ptr<H2BLBSubtarget> SubtargetSingleton;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  H2BLBTargetMachine(
      const Target &T,  //
      const Triple &TT, //
      StringRef CPU,    // cpu name
      StringRef FS,     // feature string (ex: -Xclang -target-feature -Xclang
                        // +sse2,-sse)
      const TargetOptions &Options, // sets default behavior for target (ex:
                                    // how math isnt are interpreted)
      std::optional<Reloc::Model>
          RM, // relocation model (how symbols are accessed)
      std::optional<CodeModel::Model>
          CM,             // codegen model (ex : size settings of the binary)
      CodeGenOptLevel OL, // optimisation level
      bool JIT);          // has jit
  ~H2BLBTargetMachine() override;

  const H2BLBSubtarget *getSubtargetImpl(const Function &F) const override;
  TargetTransformInfo getTargetTransformInfo(const Function &F) const override;
  TargetLoweringObjectFile *getObjFileLowering() const override;

  void registerPassBuilderCallbacks(PassBuilder &PB) override;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
};

// lass that handle what action should be done on the code with this arch
// * add pass to the ir
// * inst selector
// * etc...
class H2BLBPassConfig : public TargetPassConfig {
public:
  H2BLBPassConfig(TargetMachine &TM, PassManagerBase &PM);

  H2BLBTargetMachine &getH2BLBTargetMachine() const {
    return getTM<H2BLBTargetMachine>();
  }

  void addIRPasses() override;
  bool addInstSelector() override;
  bool addIRTranslator() override;
  bool addLegalizeMachineIR() override;
  bool addRegBankSelect() override;
  bool addGlobalInstructionSelect() override;
  void addPreLegalizeMachineIR() override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_H2BLB_H2BLBTARGETMACHINE_H
