#ifndef TOY_TARGET_MACHINE_H
#define TOY_TARGET_MACHINE_H

#include "llvm/Target/TargetMachine.h"
#include <llvm/Support/CodeGen.h>
#include <llvm/Target/TargetMachine.h>
#include <optional>

namespace llvm {
class ToyTargetMachine : public LLVMTargetMachine {
public:
  ToyTargetMachine(Target const &T, Triple const &TT, StringRef CPU,
                   StringRef FS, TargetOptions const &Options,
                   std::optional<Reloc::Model> RM,
                   std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                   bool JIT);
  ~ToyTargetMachine() {}
};

} // namespace llvm

#endif