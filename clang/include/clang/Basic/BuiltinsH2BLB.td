include "clang/Basic/BuiltinsBase.td"

def WideningSignedMultiply : TargetBuiltin {
	let Spellings = ["__builtin_h2blb_widening_smul"];
	let Attributes = [NoThrow, Const];
	let Prototype = "int(short, short)";
}

def WideningUnsignedMultiply : TargetBuiltin {
	let Spellings = ["__builtin_h2blb_widening_umul"];
	let Attributes = [NoThrow, Const];
	let Prototype = "unsigned int(unsigned short, unsigned short)";
}
