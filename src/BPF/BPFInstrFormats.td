//===-- BPFInstrFormats.td - BPF 指令格式定义 -*- tablegen -*-===//
// 
// 定义 BPF 指令的编码格式和操作类别

//===----------------------------------------------------------------------===//
// 指令操作类别（OpClass）定义
// 用于划分指令类型，对应 BPF 指令格式中的 opcode 字段高3位
// 位置：Inst{58-56}（64位指令中的位域）
//===----------------------------------------------------------------------===//
class BPFOpClass<bits<3> val> {
  bits<3> Value = val; // 3位二进制编码值
}

// 具体操作类别定义：
def BPF_LD    : BPFOpClass<0x0>; // 加载指令（如 ldw [r1 + 0], r2）
def BPF_LDX   : BPFOpClass<0x1>; // 寄存器间接加载（内存源为寄存器）
def BPF_ST    : BPFOpClass<0x2>; // 立即数存储（如 stw [r1 + 0], 42）
def BPF_STX   : BPFOpClass<0x3>; // 寄存器存储（如 stxw [r1 + 0], r2）
def BPF_ALU   : BPFOpClass<0x4>; // 32位算术逻辑运算（ALU32）
def BPF_JMP   : BPFOpClass<0x5>; // 64位跳转指令（条件/无条件）
def BPF_JMP32 : BPFOpClass<0x6>; // 32位跳转指令（用于低32位比较）
def BPF_ALU64 : BPFOpClass<0x7>; // 64位算术逻辑运算

//===----------------------------------------------------------------------===//
// 源操作数类型（SrcType）
// 标识指令源是立即数（K）还是寄存器（X）
// 位置：Inst{55}（1位）
//===----------------------------------------------------------------------===//
class BPFSrcType<bits<1> val> {
  bits<1> Value = val;
}

def BPF_K : BPFSrcType<0x0>; // 使用立即数（如 add r1, 42）
def BPF_X : BPFSrcType<0x1>; // 使用寄存器（如 add r1, r2）

//===----------------------------------------------------------------------===//
// 算术/逻辑操作码（ArithOp）
// 定义 ALU/ALU64 指令的具体操作类型
// 位置：Inst{59-56}（低4位）
//===----------------------------------------------------------------------===//
class BPFArithOp<bits<4> val> {
  bits<4> Value = val;
}

def BPF_ADD  : BPFArithOp<0x0>;  // 加法
def BPF_SUB  : BPFArithOp<0x1>;  // 减法
def BPF_MUL  : BPFArithOp<0x2>;  // 乘法
def BPF_DIV  : BPFArithOp<0x3>;  // 无符号除法
def BPF_OR   : BPFArithOp<0x4>;  // 按位或
def BPF_AND  : BPFArithOp<0x5>;  // 按位与
def BPF_LSH  : BPFArithOp<0x6>;  // 左移
def BPF_RSH  : BPFArithOp<0x7>;  // 逻辑右移
def BPF_NEG  : BPFArithOp<0x8>;  // 取反
def BPF_MOD  : BPFArithOp<0x9>;  // 取模
def BPF_XOR  : BPFArithOp<0xa>;  // 异或
def BPF_MOV  : BPFArithOp<0xb>;  // 寄存器搬移
def BPF_ARSH : BPFArithOp<0xc>;  // 算术右移（保留符号位）
def BPF_END  : BPFArithOp<0xd>;  // 字节序转换

// 原子操作扩展（BPF_XCHG/CMPXCHG）
def BPF_XCHG    : BPFArithOp<0xe>; // 原子交换
def BPF_CMPXCHG : BPFArithOp<0xf>; // 比较并交换

//===----------------------------------------------------------------------===//
// 字节序转换方向（Endian Direction）
// 配合 BPF_END 操作码使用
//===----------------------------------------------------------------------===//
class BPFEndDir<bits<1> val> {
  bits<1> Value = val;
}

def BPF_TO_LE : BPFSrcType<0x0>; // 转换为小端序
def BPF_TO_BE : BPFSrcType<0x1>; // 转换为大端序

//===----------------------------------------------------------------------===//
// 跳转指令操作码（JumpOp）
// 定义 JMP/JMP32 指令的条件类型
//===----------------------------------------------------------------------===//
class BPFJumpOp<bits<4> val> {
  bits<4> Value = val;
}

def BPF_JA   : BPFJumpOp<0x0>;  // 无条件跳转
def BPF_JEQ  : BPFJumpOp<0x1>;  // 等于（==）
def BPF_JGT  : BPFJumpOp<0x2>;  // 无符号大于（>）
def BPF_JGE  : BPFJumpOp<0x3>;  // 无符号大于等于（>=）
def BPF_JSET : BPFJumpOp<0x4>;  // 位测试（&）
def BPF_JNE  : BPFJumpOp<0x5>;  // 不等于（!=）
def BPF_JSGT : BPFJumpOp<0x6>;  // 有符号大于（s>）
def BPF_JSGE : BPFJumpOp<0x7>;  // 有符号大于等于（s>=）
def BPF_CALL : BPFJumpOp<0x8>;  // 函数调用（特殊跳转）
def BPF_EXIT : BPFJumpOp<0x9>;  // 程序退出
def BPF_JLT  : BPFJumpOp<0xa>;  // 无符号小于（<）
def BPF_JLE  : BPFJumpOp<0xb>;  // 无符号小于等于（<=）
def BPF_JSLT : BPFJumpOp<0xc>;  // 有符号小于（s<）
def BPF_JSLE : BPFJumpOp<0xd>;  // 有符号小于等于（s<=）
def BPF_JCOND : BPFJumpOp<0xe>; // 条件跳转（保留）

//===----------------------------------------------------------------------===//
// 内存操作宽度修饰符（Width Modifer）
// 指定内存访问的字节宽度（对应W/H/B/DW）
// 位置：Inst{51-50}（2位）
//===----------------------------------------------------------------------===//
class BPFWidthModifer<bits<2> val> {
  bits<2> Value = val;
}

def BPF_W  : BPFWidthModifer<0x0>; // 32位（4字节）
def BPF_H  : BPFWidthModifer<0x1>; // 16位（2字节）
def BPF_B  : BPFWidthModifer<0x2>; // 8位（1字节）
def BPF_DW : BPFWidthModifer<0x3>; // 64位（8字节）

//===----------------------------------------------------------------------===//
// 内存模式修饰符（Mode Modifer）
// 用于LD/ST类指令的寻址模式
//===----------------------------------------------------------------------===//
class BPFModeModifer<bits<3> val> {
  bits<3> Value = val;
}

def BPF_IMM  : BPFModeModifer<0x0>;  // 立即数模式（特殊用途）
def BPF_ABS  : BPFModeModifer<0x1>;  // 绝对地址加载（用于数据包访问）
def BPF_IND  : BPFModeModifer<0x2>;  // 间接地址加载（寄存器基址+立即偏移）
def BPF_MEM  : BPFModeModifer<0x3>;  // 常规内存访问
def BPF_MEMSX  : BPFModeModifer<0x4>; // 带符号扩展的内存加载
def BPF_ATOMIC : BPFModeModifer<0x6>; // 原子操作模式

//===----------------------------------------------------------------------===//
// 原子操作标志（Atomic Flag）
// 配合BPF_ATOMIC模式使用（如是否带fetch）
//===----------------------------------------------------------------------===//
class BPFAtomicFlag<bits<4> val> {
  bits<4> Value = val;
}

def BPF_FETCH : BPFAtomicFlag<0x1>; // 原子操作需要返回旧值

//===----------------------------------------------------------------------===//
// BPF指令基类（InstBPF）
// 所有具体指令继承自此类，定义公共编码结构
//===----------------------------------------------------------------------===//
class InstBPF<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  field bits<64> Inst;      // 64位指令编码
  field bits<64> SoftFail = 0; // 软失败标志（用于验证器不支持的指令）
  let Size = 8;             // 指令固定为8字节

  let Namespace = "BPF";    // 命名空间（用于C++生成）
  let DecoderNamespace = "BPF"; // 解码器命名空间

  BPFOpClass BPFClass;      // 绑定操作类别（设置Inst{58-56}）
  let Inst{58-56} = BPFClass.Value;

  dag OutOperandList = outs; // 输出操作数
  dag InOperandList = ins;   // 输入操作数
  let AsmString = asmstr;    // 汇编语法模板
  let Pattern = pattern;     // DAG模式匹配规则（用于指令选择）
}

//===----------------------------------------------------------------------===//
// 伪指令（Pseudo Instructions）
// 无实际机器码，用于中间表示或宏展开
//===----------------------------------------------------------------------===//
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : InstBPF<outs, ins, asmstr, pattern> {
  let Inst{63-0} = 0;       // 伪指令编码为全0
  let isPseudo = 1;         // 标记为伪指令
}