module {
  llvm.func @loop_add() -> i64 {
    %0 = llvm.mlir.constant(0 : i64) : i64
    %1 = builtin.unrealized_conversion_cast %0 : i64 to index
    %2 = llvm.mlir.constant(0 : i64) : i64
    %3 = builtin.unrealized_conversion_cast %2 : i64 to index
    %4 = llvm.mlir.constant(10 : i64) : i64
    %5 = builtin.unrealized_conversion_cast %4 : i64 to index
    %6 = llvm.mlir.constant(1 : i64) : i64
    %7 = builtin.unrealized_conversion_cast %6 : i64 to index
    %8 = scf.for %arg0 = %3 to %5 step %7 iter_args(%arg1 = %1) -> (index) {
      %10 = builtin.unrealized_conversion_cast %arg0 : index to i64
      %11 = builtin.unrealized_conversion_cast %arg1 : index to i64
      %12 = llvm.add %11, %10 : i64
      %13 = builtin.unrealized_conversion_cast %12 : i64 to index
      scf.yield %13 : index
    }
    %9 = builtin.unrealized_conversion_cast %8 : index to i64
    llvm.return %9 : i64
  }
  llvm.func @main() -> i32 {
    %0 = llvm.call @loop_add() : () -> i64
    %1 = llvm.trunc %0 : i64 to i32
    llvm.return %1 : i32
  }
}

