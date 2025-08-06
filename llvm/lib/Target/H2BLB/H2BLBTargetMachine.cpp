#include "H2BLBTargetMachine.h"
#include "H2BLB.h"
#include "H2BLBSubtarget.h"
#include "H2BLBTargetTransformInfo.h"
#include "TargetInfo/H2BLBTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Passes/PassBuilder.h"
#include <memory>

using namespace llvm;

// look at chapter 7 to understand this
static const char *H2BLBDataLayoutStr =
    "e-p:16:32:32-n16:32-i1:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-v32:32:"
    "32";

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

const H2BLBSubtarget *
H2BLBTargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  StringRef CPU = CPUAttr.isValid() ? CPUAttr.getValueAsString() : TargetCPU;
  StringRef FS = FSAttr.isValid() ? FSAttr.getValueAsString() : TargetFS;

  if (!SubtargetSingleton)
    SubtargetSingleton =
        std::make_unique<H2BLBSubtarget>(TargetTriple, CPU, FS, *this);
  return SubtargetSingleton.get();
}

TargetTransformInfo
H2BLBTargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(std::make_unique<H2BLBTTIImpl>(this, F));
}

void H2BLBTargetMachine::registerPassBuilderCallbacks(PassBuilder &PB) {
  // old passs manager
#define GET_PASS_REGISTRY "H2BLBPassRegistry.def"
#include "llvm/Passes/TargetPassRegistry.inc"

  // new pass manager
  PB.registerPipelineStartEPCallback(
      [](ModulePassManager &MPM, OptimizationLevel OptLevel) {
        // Do not add optimization passes if we are in O0.
        if (OptLevel == OptimizationLevel::O0)
          return;
        FunctionPassManager FPM;
        FPM.addPass(H2BLBSimpleConstantPropagationNewPass());
        MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
      });
}

TargetPassConfig *H2BLBTargetMachine::createPassConfig(PassManagerBase &PM) {
	return new H2BLBPassConfig(*this, PM);
}

///
/// H2BLBPassConfig
///

H2BLBPassConfig::H2BLBPassConfig(TargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

void H2BLBPassConfig::addIRPasses() {
	TargetPassConfig::addIRPasses();
	if (getOptLevel() != CodeGenOptLevel::None) {
		addPass(createH2BLBSimpleConstantPropagationPassForLegacyPM());
	}
}

///
/// EXTERN
///

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeH2BLBTarget() {
  RegisterTargetMachine<H2BLBTargetMachine> X(llvm::getTheH2BLBTarget());
}
