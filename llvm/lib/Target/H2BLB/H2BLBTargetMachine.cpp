#include "H2BLBTargetMachine.h"
#include "H2BLB.h"
#include "H2BLBSubtarget.h"
#include "H2BLBTargetObjectFile.h"
#include "H2BLBTargetTransformInfo.h"
#include "TargetInfo/H2BLBTargetInfo.h"
#include "llvm/CodeGen/GlobalISel/IRTranslator.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelect.h"
#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/RegBankSelect.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/PassRegistry.h"
#include "llvm/Passes/PassBuilder.h"
#include <memory>

using namespace llvm;

// look at chapter 7 to understand this
static const char *H2BLBDataLayoutStr =
    "e-p:16:32:32-n16:32-i1:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-v32:32:"
    "32";

static std::unique_ptr<TargetLoweringObjectFile> createTLOF(const Triple &TT) {
  if (TT.isOSBinFormatELF())
    return std::make_unique<H2BLB_ELFTargetObjectFile>();
  if (TT.isOSBinFormatMachO())
    return std::make_unique<H2BLB_MachoOTargetObjectFile>();
  // Other format not supported yet.
  return nullptr;
}

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
                               CM ? *CM : CodeModel::Small, OL),
      TLOF(createTLOF(getTargetTriple())) {
  initAsmInfo();
  setGlobalISel(true);
}

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

TargetLoweringObjectFile *H2BLBTargetMachine::getObjFileLowering() const {
  return TLOF.get();
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

bool H2BLBPassConfig::addInstSelector() {
  // addPass(createH2BLBISelDAG(getH2BLBTargetMachine()));
  return false;
}

bool H2BLBPassConfig::addIRTranslator() {
	addPass(new IRTranslator(getOptLevel()));
	return false;
}

bool H2BLBPassConfig::addLegalizeMachineIR() {
	addPass(new Legalizer());
	return false;
}

bool H2BLBPassConfig::addRegBankSelect() {
	addPass(new RegBankSelect());
	return false;
}

bool H2BLBPassConfig::addGlobalInstructionSelect() {
	addPass(new InstructionSelect(getOptLevel()));
	return false;
}

void H2BLBPassConfig::addPreLegalizeMachineIR() {
	addPass(createH2BLBMandatoryPreLegalizerCombiner());
}

///
/// EXTERN
///

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeH2BLBTarget() {
  RegisterTargetMachine<H2BLBTargetMachine> X(llvm::getTheH2BLBTarget());

	PassRegistry &PR = *PassRegistry::getPassRegistry();
	initializeH2BLBSimpleConstantPropagationPass(PR);
	initializeGlobalISel(PR);
}
