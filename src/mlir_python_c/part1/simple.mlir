// 在llvm中写循环比较麻烦，必须手动处理blocks、phi nodes这些
//  就为实现一个基础的循环，而MLIR允许我们直接使用高级结构（如循环语句）
// 通过一个pass就能lower为LLVM IR

func.func @loop_add() -> (index) {
    %init = index.constant 0
    %lb = index.constant 0
    %ub = index.constant 10
    %step = index.constant 1

    %sum = scf.for %iv = %lb to %ub step %step iter_args(%acc = %init) -> (index) {
        %sun_next = arith.addi %acc, %iv : index
        scf.yield %sun_next : index
    }

    return %sum : index
}

func.func @main() -> (i32) {
    %out = call @loop_add() : () -> (index)
    %out_i32 = arith.index_cast %out : index to i32
    func.return %out_i32 : i32
}
