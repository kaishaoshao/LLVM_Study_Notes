module {
  llvm.func @loop_add() -> i64 {
    %idx0 = index.constant 0
    %idx0_0 = index.constant 0
    %idx10 = index.constant 10
    %idx1 = index.constant 1
    %0 = scf.for %arg0 = %idx0_0 to %idx10 step %idx1 iter_args(%arg1 = %idx0) -> (index) {
      %2 = builtin.unrealized_conversion_cast %arg0 : index to i64
      %3 = builtin.unrealized_conversion_cast %arg1 : index to i64
      %4 = llvm.add %3, %2 : i64
      %5 = builtin.unrealized_conversion_cast %4 : i64 to index
      scf.yield %5 : index
    }
    %1 = builtin.unrealized_conversion_cast %0 : index to i64
    llvm.return %1 : i64
  }
  llvm.func @main() -> i32 {
    %0 = llvm.call @loop_add() : () -> i64
    %1 = llvm.trunc %0 : i64 to i32
    llvm.return %1 : i32
  }
}

