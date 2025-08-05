let TargetPrefix = "h2blb" in {

class H2BLB_Intrinsic<string suffix, list<LLVMType> ret_types, list<LLVMType> param_types>
	: ClangBuiltin<!strconcat("__builtin_h2blb_", suffix)>,
	DefaultAttrsIntrinsic<ret_types, param_types>;

def int_h2blb_widening_smul :
	H2BLB_Intrinsic<"widening_smul", [llvm_i32_ty], [llvm_i16_ty, llvm_i16_ty]>;

def int_h2blb_widening_umul :
	H2BLB_Intrinsic<"widening_umul", [llvm_i32_ty], [llvm_i16_ty, llvm_i16_ty]>;

}
