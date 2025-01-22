# **Cpu0概述**

**Cpu0** 是一个用于教学和学习的简化处理器架构，通常用于 LLVM 后端开发的教程中。它的设计目标是简单易懂，同时涵盖现代处理器的核心概念。以下是 **Cpu0 指令架构** 的详细介绍：

## 概述

### **1. Cpu0 架构概述**

- **位宽**：32 位架构（支持 32 位寄存器和地址空间）。
- **字节序**：支持大端（`cpu0`）和小端（`cpu0el`）。
- **寄存器**：16 个通用寄存器（GPR）和少量特殊寄存器。
- **指令集**：精简指令集（RISC），指令格式固定。
- **内存访问**：支持加载（Load）和存储（Store）指令。
- **分支和跳转**：支持条件分支和无条件跳转。

---

### **2. 寄存器**

Cpu0 的寄存器分为两类：**通用寄存器** 和 **特殊寄存器**。

#### **2.1 通用寄存器（GPR）**

- **数量**：16 个（32 位）。
- **命名**：
  - `ZERO`：始终为 0，用于常量操作。
  - `AT`：汇编临时寄存器。
  - `V0`, `V1`：函数返回值寄存器。
  - `A0`, `A1`：函数参数寄存器。
  - `T0`, `T1`, `T9`：临时寄存器。
  - `S0`, `S1`：保存寄存器（跨函数调用保持不变）。
  - `GP`：全局指针寄存器。
  - `FP`：帧指针寄存器。
  - `SP`：栈指针寄存器。
  - `LR`：链接寄存器（用于函数返回地址）。
  - `SW`：状态寄存器。

#### **2.2 特殊寄存器**

- **PC**：程序计数器，指向当前指令地址。
- **EPC**：异常程序计数器，用于异常处理。

---

### **3. 指令格式**

Cpu0 的指令格式固定为 32 位，分为以下几种类型：

* opcode : 操作码，用于标识指令的类型
* ra           : 目标寄存器，只用于3寄存器指令
* rb           :  源寄存器
* rc           ：源寄存器
* cx           ：立即数

#### **3.1 A 型指令（寄存器-寄存器）**

- **格式**：`opcode(8) | ra(4) | rb(4) | rc(4) | cx(12)`

- **示例**：
  
  -  
  
- td定义

  ```cpp
  class FA<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern,
  		InstrItinClass itin>: Cp
  ```

  

#### **3.2 I 型指令（立即数）**

- **格式**：`opcode(6) | rs(5) | rt(5) | imm(16)`
- **示例**：
  - 

#### **3.3 J 型指令（跳转）**

- **格式**：`opcode(8) | cx(24)`

- **示例**：
  - 
  
    

---

### **4. 指令集**

Cpu0 的指令集包括以下几类：

#### **4.1 算术和逻辑指令**

- **ADD**：加法。
- **SUB**：减法。
- **ADDI**：加立即数。
- **AND**：按位与。
- **OR**：按位或。
- **XOR**：按位异或。

#### **4.2 内存访问指令**

- **LW**：从内存加载字（32 位）。
- **SW**：存储字到内存。

#### **4.3 分支和跳转指令**

- **BEQ**：如果相等则分支。
- **BNE**：如果不相等则分支。
- **J**：无条件跳转。
- **JAL**：跳转并链接（用于函数调用）。

#### **4.4 特殊指令**

- **NOP**：空操作。
- **RET**：函数返回（跳转到 `LR` 中的地址）。

---

### **5. 内存模型**

- **地址空间**：32 位地址空间（4GB）。
- **对齐**：字（32 位）必须按 4 字节对齐。
- **栈**：栈指针（`SP`）指向栈顶，栈向低地址增长。

---

### **6. 异常处理**

- **EPC**：保存异常发生时的指令地址。
- **SW**：状态寄存器，包含异常状态信息。

---

### **7. 示例代码**

以下是一个简单的 Cpu0 汇编程序示例：

```assembly
    .text
    .globl main
main:
    ADDI $t0, $zero, 10      # t0 = 10
    ADDI $t1, $zero, 20      # t1 = 20
    ADD  $t2, $t0, $t1       # t2 = t0 + t1
    SW   $t2, 0($sp)         # 将 t2 存储到栈中
    LW   $t3, 0($sp)         # 从栈中加载数据到 t3
    RET                      # 返回
```

---

### **8. Cpu0 在 LLVM 中的实现**

在 LLVM 中，Cpu0 的后端实现包括：

- **目标描述文件（.td 文件）**：定义指令集、寄存器、调度信息等。
- **C++ 代码**：实现指令选择、寄存器分配、代码生成等。
- **测试用例**：验证后端的正确性。

### **9. 总结**

Cpu0 是一个简化的 32 位 RISC 架构，适合用于教学和 LLVM 后端开发的学习。它的指令集和寄存器设计简单明了，同时涵盖了现代处理器的核心概念。通过实现 Cpu0 后端，可以深入理解 LLVM 的工作原理和目标代码生成的过程。

## tableGen





### **Cpu0 TableGen 文件**

#### **1. 定义寄存器（Cpu0RegisterInfo.td）**

```tablegen
// 定义寄存器的父类
class Cpu0Reg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc; // 硬件编码
  let Namespace = "Cpu0"; // 命名空间
}

// 定义通用寄存器
def ZERO : Cpu0Reg<0,  "zero">, DwarfRegNum<[0]>; // 零寄存器
def AT   : Cpu0Reg<1,  "at">,   DwarfRegNum<[1]>; // 汇编临时寄存器
def V0   : Cpu0Reg<2,  "v0">,   DwarfRegNum<[2]>; // 返回值寄存器 0
def V1   : Cpu0Reg<3,  "v1">,   DwarfRegNum<[3]>; // 返回值寄存器 1
def A0   : Cpu0Reg<4,  "a0">,   DwarfRegNum<[4]>; // 参数寄存器 0
def A1   : Cpu0Reg<5,  "a1">,   DwarfRegNum<[5]>; // 参数寄存器 1
def T0   : Cpu0Reg<6,  "t0">,   DwarfRegNum<[6]>; // 临时寄存器 0
def T1   : Cpu0Reg<7,  "t1">,   DwarfRegNum<[7]>; // 临时寄存器 1
def T9   : Cpu0Reg<8,  "t9">,   DwarfRegNum<[8]>; // 临时寄存器 9
def S0   : Cpu0Reg<9,  "s0">,   DwarfRegNum<[9]>; // 保存寄存器 0
def S1   : Cpu0Reg<10, "s1">,   DwarfRegNum<[10]>; // 保存寄存器 1
def GP   : Cpu0Reg<11, "gp">,   DwarfRegNum<[11]>; // 全局指针寄存器
def FP   : Cpu0Reg<12, "fp">,   DwarfRegNum<[12]>; // 帧指针寄存器
def SP   : Cpu0Reg<13, "sp">,   DwarfRegNum<[13]>; // 栈指针寄存器
def LR   : Cpu0Reg<14, "lr">,   DwarfRegNum<[14]>; // 链接寄存器
def SW   : Cpu0Reg<15, "sw">,   DwarfRegNum<[15]>; // 状态寄存器

// 定义特殊寄存器
def PC   : Cpu0Reg<16, "pc">,   DwarfRegNum<[16]>; // 程序计数器
def EPC  : Cpu0Reg<17, "epc">,  DwarfRegNum<[17]>; // 异常程序计数器

// 定义寄存器类
def CPURegs : RegisterClass<"Cpu0", [i32], 32, (add
  ZERO, AT, V0, V1, A0, A1, T0, T1, T9, S0, S1, GP, FP, SP, LR, SW
)>;

def SR     : RegisterClass<"Cpu0", [i32], 32, (add SW)>; // 状态寄存器类
def C0Regs : RegisterClass<"Cpu0", [i32], 32, (add PC, EPC)>; // 特殊寄存器类
```

---

#### **2. 定义指令集（Cpu0InstrInfo.td）**

```tablegen
// 包含目标描述文件
include "Cpu0RegisterInfo.td"

// 定义指令格式
class FL<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  bits<8> Opcode = op;
  dag OutOperandList = outs;
  dag InOperandList = ins;
  string AsmString = asmstr;
  list<dag> Pattern = pattern;
  let Namespace = "Cpu0";
}

// 定义算术指令
class ArithLogicI<bits<8> op, string instr_asm, SDNode OpNode, Operand Od>
  : FL<op, (outs GPROut:$ra), (ins CPURegs:$rb, Od:$imm16),
       !strconcat(instr_asm, "\t$ra, $rb, $imm16"),
       [(set GPROut:$ra, (OpNode CPURegs:$rb, Od:$imm16))]>;

// 定义加载/存储指令
class LoadM<bits<8> op, string instr_asm, PatFrag OpNode, RegisterClass RC,
            Operand MemOpnd>
  : FL<op, (outs RC:$ra), (ins MemOpnd:$addr),
       !strconcat(instr_asm, "\t$ra, $addr"),
       [(set RC:$ra, (OpNode addr:$addr))]>;

class StoreM<bits<8> op, string instr_asm, PatFrag OpNode, RegisterClass RC,
             Operand MemOpnd>
  : FL<op, (outs), (ins RC:$ra, MemOpnd:$addr),
       !strconcat(instr_asm, "\t$ra, $addr"),
       [(OpNode RC:$ra, addr:$addr)]>;

// 定义具体指令
def ADDiu : ArithLogicI<0x09, "addiu", add, simm16>; // 加立即数
def LW    : LoadM<0x23, "lw", load, CPURegs, mem>;   // 加载字
def SW    : StoreM<0x2B, "sw", store, CPURegs, mem>; // 存储字
def JR    : FL<0x3C, (outs), (ins GPROut:$ra), "jr\t$ra", [(brind GPROut:$ra)]>; // 跳转
def RET   : FL<0x3C, (outs), (ins GPROut:$ra), "ret", [(retflag)]>; // 返回
```

---

#### **3. 定义调度信息（Cpu0Schedule.td）**

```tablegen
// 定义功能单元
def ALU : FuncUnit; // 算术逻辑单元

// 定义指令调度类
def IIAlu   : InstrItinClass; // 算术指令
def IILoad  : InstrItinClass; // 加载指令
def IIStore : InstrItinClass; // 存储指令
def IIBranch: InstrItinClass; // 分支指令

// 定义调度信息
def Cpu0GenericItineraries : ProcessorItineraries<[ALU], [], [
  InstrItinData<IIAlu,    [InstrStage<1, [ALU]>]>, // 算术指令 1 周期
  InstrItinData<IILoad,   [InstrStage<3, [ALU]>]>, // 加载指令 3 周期
  InstrItinData<IIStore,  [InstrStage<1, [ALU]>]>, // 存储指令 1 周期
  InstrItinData<IIBranch, [InstrStage<1, [ALU]>]>  // 分支指令 1 周期
]>;
```

---

#### **4. 定义目标描述（Cpu0.td）**

```tablegen
// 包含目标描述文件
include "Cpu0RegisterInfo.td"
include "Cpu0InstrInfo.td"
include "Cpu0Schedule.td"

// 定义目标
def Cpu0 : Target {
  let InstructionSet = Cpu0InstrInfo; // 指令集
  let RegisterInfo = Cpu0RegisterInfo; // 寄存器信息
  let Itineraries = Cpu0GenericItineraries; // 调度信息
}
```

---

### **5. 生成 C++ 代码**

将上述 `.td` 文件保存到 `llvm/lib/Target/Cpu0` 目录中，然后使用 `llvm-tblgen` 生成 C++ 代码：

```bash
llvm-tblgen -gen-register-info Cpu0.td -o Cpu0GenRegisterInfo.inc
llvm-tblgen -gen-instr-info Cpu0.td -o Cpu0GenInstrInfo.inc
llvm-tblgen -gen-subtarget Cpu0.td -o Cpu0GenSubtargetInfo.inc
```
