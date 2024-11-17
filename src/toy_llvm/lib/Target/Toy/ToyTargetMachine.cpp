#include "ToyTargetMachine.h"

#include "llvm/MC/TargetRegistry.h"
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Target/TargetOptions.h>

using namespace llvm;

extern Target TheToyTarget;

extern "C" void LLVMInitialToyTarget(){
    llvm::RegisterTargetMachine<ToyTargetMachine> X(TheToyTarget);
}

ToyTargetMachine::ToyTargetMachine(Target const &T,Triple const &TT,
                                   StringRef CPU,StringRef FS,
                                   TargetOptions const &Options,
                                   std::optional<Reloc::Model> RM,
                                   std::optional<CodeModel::Model> CM,
                                   CodeGenOptLevel OL,bool JIT)
// 这里的参数主要是llc传递过来的命令行参数
: LLVMTargetMachine(T,"",TT,CPU,FS,Options,Reloc::Static,CodeModel::Small,OL){
    initAsmInfo();
}