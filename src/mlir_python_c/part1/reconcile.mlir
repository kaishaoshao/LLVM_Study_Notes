module {
  llvm.func @loop_add() -> i64 {
    %0 = llvm.mlir.constant(0 : i64) : i64
    %1 = builtin.unrealized_conversion_cast %0 : i64 to index
    %2 = llvm.mlir.constant(0 : i64) : i64
    %3 = builtin.unrealized_conversion_cast %2 : i64 to index
    %4 = llvm.mlir.constant(10 : i64) : i64
    %5 = llvm.mlir.constant(1 : i64) : i64
    cf.br ^bb1(%3, %1 : index, index)
  ^bb1(%6: index, %7: index):  // 2 preds: ^bb0, ^bb2
    %8 = builtin.unrealized_conversion_cast %6 : index to i64
    %9 = llvm.icmp "slt" %8, %4 : i64
    llvm.cond_br %9, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %10 = builtin.unrealized_conversion_cast %6 : index to i64
    %11 = builtin.unrealized_conversion_cast %7 : index to i64
    %12 = llvm.add %11, %10 : i64
    %13 = builtin.unrealized_conversion_cast %12 : i64 to index
    %14 = llvm.add %8, %5 : i64
    %15 = builtin.unrealized_conversion_cast %14 : i64 to index
    cf.br ^bb1(%15, %13 : index, index)
  ^bb3:  // pred: ^bb1
    %16 = builtin.unrealized_conversion_cast %7 : index to i64
    llvm.return %16 : i64
  }
  llvm.func @main() -> i32 {
    %0 = llvm.call @loop_add() : () -> i64
    %1 = llvm.trunc %0 : i64 to i32
    llvm.return %1 : i32
  }
}

