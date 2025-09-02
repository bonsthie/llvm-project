this follow the chapter 9 to 21 `H2BLB` backend creation

to use it you need :
* add the `H2BLB` to the target list
* put this folder in `llvm/lib/Target/H2BLB`
* add the H2BLB in the `TargetParser/Triple.h`
```cpp
<snip>
class Triple {
    h2blb, 
}
<snip>

```
* add the H2BLB in the `TargetParser/Triple.cpp`
```cpp
static Triple::ObjectFormatType getDefaultFormat(const Triple &T) {
    switch (T.getArch()) {
        case Triple::h2blb:
            switch (T.getOS()) {
                case Triple::Win32:
                case Triple::UEFI:
                    return Triple::COFF;
        default:
            return T.isOSDarwin() ? Triple::MachO : Triple::ELF;
}

<snip>
unsigned Triple::getArchPointerBitWidth(llvm::Triple::ArchType Arch) {
    switch (Arch) {
        case llvm::Triple::h2blb:
            return 16;
<snip>

Triple Triple::get32BitArchVariant() const {
    Triple T(*this);
    switch (getArch()) {
        case llvm::Triple::h2blb:
            // This is already in 32-bit
            return T;
<snip>

<snip>
Triple::get64BitArchVariant() const {
  case Triple::h2blb:
    T.setArch(UnknownArch);
    break;
<snip>

<snip>
Triple::getArchTypeName(ArchType Kind) {
     case h2blb:          return "h2blb";
}
<snip>

<snip>
Triple::getArchTypePrefix(ArchType Kind) {
     case h2blb:          return "h2blb";
<snip>

<snip>
Triple::getArchTypeForLLVMName(StringRef Name) {
          .Case("h2blb", h2blb)
<snip>

<snip>
 static Triple::ArchType parseArch(StringRef ArchName) {
          .Case("h2blb", h2blb)
<snip>


```
* add the Target to the `clang/lib/Basic/Targets/` H2BLB.{cpp/.h}
* add them to the cmakelist of the folder
* update `clang/lib/Basic/Targets.cpp`

```cpp
<snip>
case llvm::Triple::h2blb:
    return std::make_unique<H2BLBTargetInfo>(Triple, Opts);
<snip>

```

* add the intrinsic in `llvm/include/llvm/IR/IntrinsicH2BLB.td`
* add the `IntrinsicH2BLB.td` in `llvm/include/llvm/IR/Intrinsic.td`
* add the `IntrinsicH2BLB.td` in the cmake for tablegen `tablegen(LLVM IntrinsicsH2BLB.h -gen-intrinsic-enums -intrinsic-prefix=h2blb)`
* include `llvm/IR/IntrinsicsH2BLB.h` to Intrinsics.cpp
* create the `clang/include/clang/Basic/BuiltinsH2BLB.td`
