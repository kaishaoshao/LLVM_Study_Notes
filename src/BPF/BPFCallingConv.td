//===-- BPFCallingConv.td - BPF 调用约定定义 -*- tablegen -*-===//
// 
// 定义 BPF 架构的函数参数传递规则和返回值处理约定

// BPF 64位架构的返回值约定
def RetCC_BPF64 : CallingConv<[
  // 如果返回值类型是 i64，分配到寄存器 R0
  CCIfType<[i64], CCAssignToReg<[R0]>>
]>;

// BPF 64位架构的函数参数传递约定
def CC_BPF64 : CallingConv<[
  // 将 i8/i16/i32 类型参数提升为 i64（BPF 默认用 64 位寄存器操作）
  CCIfType<[i8, i16, i32], CCPromoteToType<i64>>,

  // 优先使用寄存器传递 i64 参数（顺序：R1-R5）
  CCIfType<[i64], CCAssignToReg<[R1, R2, R3, R4, R5]>>,

  // 理论上参数可分配到 8 字节对齐的栈空间（实际 BPF 通常禁用栈传参）
  CCAssignToStack<8, 8>  // 参数栈分配单位 8 字节，对齐 8 字节
]>;

// ALU32 模式（32位操作）下的返回值约定
def RetCC_BPF32 : CallingConv<[
  // i32 返回值使用 W0 寄存器（对应 R0 的低32位）
  CCIfType<[i32], CCAssignToRegWithShadow<[W0], [R0]>>,
  
  // i64 返回值仍使用 R0 寄存器（同时标记 W0 为影子寄存器）
  CCIfType<[i64], CCAssignToRegWithShadow<[R0], [W0]>>
]>;

// ALU32 模式下的函数参数传递约定
def CC_BPF32 : CallingConv<[
  // 32位参数使用 W1-W5 寄存器（对应 R1-R5 的低32位）
  CCIfType<[i32], CCAssignToRegWithShadow<[W1, W2, W3, W4, W5],
                                          [R1, R2, R3, R4, R5]>>,

  // 64位参数仍使用 R1-R5 寄存器（同时标记对应的 W 寄存器）
  CCIfType<[i64], CCAssignToRegWithShadow<[R1, R2, R3, R4, R5],
                                          [W1, W2, W3, W4, W5]>>,

  // 栈传参规则同上（实际通常不用）                                          
  CCAssignToStack<8, 8>
]>;

// 被调用者保存的寄存器列表（Callee-Saved Registers）
// R6-R10 在函数调用中必须由被调用者保存/恢复
def CSR : CalleeSavedRegs<(add R6, R7, R8, R9, R10)>;