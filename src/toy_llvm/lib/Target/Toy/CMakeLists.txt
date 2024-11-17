add_llvm_component_group(Toy)

add_llvm_target(ToyCodeGen
    ToyTargetMachine.cpp
)

set(LLVM_TARGET_DEFINITIONS Toy.td)

tablegen(LLVM ToyGenSubtargetInfo.inc -gen-subtarget)

# add_publibc_tablegen_target(ToyCommonTableGen)

add_subdirectory(TargetInfo)
add_subdirectory(TargetDesc)