include "llvm/Target/GlobalISel/Combine.td"

def registers_matchinfo: GIDefMatchData<"SmallVector<Register>">;

def insertvectorelt_to_build_vector : GICombineRule<
  (defs root:$root, registers_matchinfo:$matchinfo),
  (match (wip_match_opcode G_INSERT_VECTOR_ELT):$root,
         [{ return matchInsertVectorElt(*${root}, ${matchinfo}); }]),
  (apply [{ applyInsertVectorElt(*${root}, ${matchinfo}); }])>;

def H2BLBMandatoryPreLegalizerCombiner: GICombiner<
  "H2BLBMandatoryPreLegalizerCombinerImpl", [optnone_combines, insertvectorelt_to_build_vector]> {
}
