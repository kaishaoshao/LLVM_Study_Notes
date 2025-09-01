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
    cf.br ^bb1(%3, %1 : index, index)
  ^bb1(%8: index, %9: index):  // 2 preds: ^bb0, ^bb2
    %10 = arith.cmpi slt, %8, %5 : index
    llvm.cond_br %10, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %11 = builtin.unrealized_conversion_cast %8 : index to i64
    %12 = builtin.unrealized_conversion_cast %9 : index to i64
    %13 = llvm.add %12, %11 : i64
    %14 = builtin.unrealized_conversion_cast %13 : i64 to index
    %15 = arith.addi %8, %7 : index
    cf.br ^bb1(%15, %14 : index, index)
  ^bb3:  // pred: ^bb1
    %16 = builtin.unrealized_conversion_cast %9 : index to i64
    llvm.return %16 : i64
  }
  llvm.func @main() -> i32 {
    %0 = llvm.call @loop_add() : () -> i64
    %1 = llvm.trunc %0 : i64 to i32
    llvm.return %1 : i32
  }
}

