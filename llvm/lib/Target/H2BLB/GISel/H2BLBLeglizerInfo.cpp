#include "H2BLBLeglizerInfo.h"
#include "H2BLBInstrInfo.h"
#include "H2BLBSubtarget.h"
#include "llvm/CodeGen/GlobalISel/LegalizerHelper.h"
#include "llvm/CodeGen/GlobalISel/MIPatternMatch.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/CodeGenTypes/LowLevelType.h"

using namespace llvm;
using namespace LegalizeActions;
using namespace MIPatternMatch;

#define DEBUG_TYPE "h2blb-legalinfo"

H2BLBLegalizerInfo::H2BLBLegalizerInfo(const H2BLBSubtarget &ST) : ST(ST) {
  const LLT p0 = LLT::pointer(0, 16);
  const LLT s8 = LLT::scalar(8);
  const LLT s16 = LLT::scalar(16);
  const LLT s32 = LLT::scalar(32);
  const LLT v2s16 [[maybe_unused]] = LLT::fixed_vector(2, 16);

  // Constants
  getActionDefinitionsBuilder(
      {TargetOpcode::G_CONSTANT, TargetOpcode::G_IMPLICIT_DEF})
      .legalFor({p0, s16, s32})
      .widenScalarToNextPow2(0)  // to the next pow of two starting form 0
      .clampScalar(0, s16, s32); // min is s16 max is s32 starting from 0

  // load and store
  // normal this is convenient to reuse the smae rules for all loads and store
  getActionDefinitionsBuilder({TargetOpcode::G_LOAD, TargetOpcode::G_SEXTLOAD,
                               TargetOpcode::G_ZEXTLOAD, TargetOpcode::G_STORE})
      // Legal when the tuple {result type, pointer type, stored memory type,
      // alignment} matches
      .legalForTypesWithMemDesc(
          {{s8, p0, s8, 8},     // byte load/store, byte-aligned
           {s16, p0, s8, 8},    // extending load / truncating store
           {s16, p0, s16, 8},   // 16 bit
           {s32, p0, s32, 8}})  // 32 bit
      .clampScalar(0, s16, s32) // min is s16 max is s32 starting from 0
      .lowerIf([=](const LegalityQuery &Query) {
        return Query.Types[0].isScalar() &&
               Query.Types[0] != Query.MMODescrs[0].MemoryTy;
      })
      // load/truncate the smaller memory type, then extend it to the result
      // type.
      .legalIf([=](const LegalityQuery &Query) {
        TypeSize Size = Query.Types[0].getSizeInBits();
        return Size == 16 || Size == 32;
      })
      .scalarize(0) // split load starting from 0
      .lower();     // the instr is already generic

  getActionDefinitionsBuilder(TargetOpcode::G_FRAME_INDEX).legalFor({p0});
  getActionDefinitionsBuilder(TargetOpcode::G_PTR_ADD).legalFor({{p0, s16}});

  // Arithmetic.
  getActionDefinitionsBuilder({TargetOpcode::G_ADD, TargetOpcode::G_AND,
                               TargetOpcode::G_SHL, TargetOpcode::G_OR})
      .legalFor({s16, s32})
      .clampScalar(0, s16, s32)
      .scalarize(0);

  getActionDefinitionsBuilder(TargetOpcode::G_MUL)
      .customIf([=](const LegalityQuery &Query) {
        const auto &DstTy = Query.Types[0];
        return !DstTy.isVector() && DstTy.getSizeInBits() == 32;
      });

  // Floating-point arithmetic.
  // call the lib starting from size 0 because there is no floating point op in
  // this arch
  getActionDefinitionsBuilder(TargetOpcode::G_FADD).scalarize(0).libcall();

  // Merge/Unmerge
  for (unsigned Op :
       {TargetOpcode::G_MERGE_VALUES, TargetOpcode::G_UNMERGE_VALUES,
        TargetOpcode::G_BUILD_VECTOR}) {
    unsigned BigTyIdx = Op == TargetOpcode::G_UNMERGE_VALUES ? 1 : 0;
    unsigned LitTyIdx = Op == TargetOpcode::G_UNMERGE_VALUES ? 0 : 1;
    getActionDefinitionsBuilder(Op).legalIf([=](const LegalityQuery &Q) {
      return Q.Types[BigTyIdx].getSizeInBits() == 32 &&
             Q.Types[LitTyIdx].getSizeInBits() == 16;
    });
  }

  getActionDefinitionsBuilder(TargetOpcode::G_EXTRACT_VECTOR_ELT)
      .legalIf([=](const LegalityQuery &Q) {
        return Q.Types[0].getSizeInBits() == 16 &&
               Q.Types[1].getSizeInBits() == 32;
      });

  getLegacyLegalizerInfo().computeTables();
}

bool H2BLBLegalizerInfo::legalizeCustom(
    LegalizerHelper &Helper, MachineInstr &MI,
    LostDebugLocObserver &LocObserver) const {
  MachineIRBuilder &MIRBuilder = Helper.MIRBuilder;
  MachineRegisterInfo &MRI = *MIRBuilder.getMRI();
  GISelChangeObserver &Observer = Helper.Observer;
  switch (MI.getOpcode()) {
  default:
    // No idea what to do.
    return false;
  case TargetOpcode::G_MUL:
    return legalizeMul(MI, MRI, MIRBuilder, Observer);
  }
  llvm_unreachable("expected switch to return");
}

bool H2BLBLegalizerInfo::legalizeMul(MachineInstr &MI, MachineRegisterInfo &MRI,
                                     MachineIRBuilder &MIRBuilder,
                                     GISelChangeObserver &Observer) const {
  assert(MI.getOpcode() == TargetOpcode::G_MUL);

  Register ValReg = MI.getOperand(0).getReg();
  const LLT ValTy = MRI.getType(ValReg);
  (void)ValTy;

  assert(ValTy == LLT::scalar(32) &&
         "Custom legalization description doesn't match implementation");

  Register LHS = MI.getOperand(0).getReg();
  Register RHS = MI.getOperand(1).getReg();
  Register PlainLHS, PlainRHS;
  bool IsSigned = false;

  // if same register type and if sign or unsigned
  if (mi_match(LHS, MRI, m_GSExt(m_Reg(PlainLHS))) &&
      mi_match(RHS, MRI, m_GSExt(m_Reg(PlainRHS))))
    IsSigned = true;

  else if (mi_match(LHS, MRI, m_GZExt(m_Reg(PlainLHS))) &&
           mi_match(RHS, MRI, m_GZExt(m_Reg(PlainRHS))))
    IsSigned = false;
  else
    return false;

  LLT s16 = LLT::scalar(16);
  if (MRI.getType(PlainLHS) != s16 || MRI.getType(PlainRHS) != s16)
    return false;

  const TargetInstrInfo &TII = *ST.getInstrInfo();
  unsigned Opcode = IsSigned ? H2BLB::WIDENING_SMUL : H2BLB::WIDENING_UMUL;
  Observer.changingInstr(MI);

  MI.setDesc(TII.get(Opcode));
  auto UpdateOperand = [](MachineOperand &MO, Register NewReg) {
    MO.setReg(NewReg);
    // The previous operand may have been the last use of the previous register.
    // This may not be the case of the NewReg, so conservatively drop the last
    // use flag.
    MO.setIsKill(false);
  };
  UpdateOperand(MI.getOperand(1), PlainLHS);
  UpdateOperand(MI.getOperand(2), PlainRHS);
  constrainSelectedInstRegOperands(MI, TII, *MRI.getTargetRegisterInfo(),
                                   *ST.getRegBankInfo());

  Observer.changedInstr(MI);
  return true;
}
