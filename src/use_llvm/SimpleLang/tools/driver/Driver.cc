#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include "SimpleLang/Basic/Version.h"

int main(int argc,char **argv)
{
    llvm::InitLLVM X(argc,argv);
    llvm::outs()<< "Hello World! (version "<<simplelang::getSimpleLangVersion()<<")\n";
    return 0;
}