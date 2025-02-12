# BPF概序

## BPF 信息

### 1. **BPF 指令集**

BPF 的指令集是一种基于寄存器的虚拟机指令集，每条指令长度为 64 位，采用固定格式。指令格式如下：

复制

```
| 8 bits | 4 bits | 4 bits | 16 bits | 32 bits |
|--------|--------|--------|---------|---------|
|  opcode |  dest  |  src   |  offset | immediate |
```

* **opcode** ：操作码，指定指令的类型（如加载、存储、算术运算等）。
* **dest** ：目标寄存器（通常为 R0-R10）。
* **src** ：源寄存器（通常为 R0-R10）。
* **offset** ：偏移量，用于内存访问或跳转指令。
* **immediate** ：立即数，用于常量操作。

#### 主要指令类型

1. **加载/存储指令** ：

* 从内存加载数据到寄存器（`LD`、`LDX`）。
* 将寄存器数据存储到内存（`ST`、`STX`）。

1. **算术/逻辑指令** ：

* 加法（`ADD`）、减法（`SUB`）、乘法（`MUL`）、除法（`DIV`）。
* 位操作（`AND`、`OR`、`XOR`、`LSH`、`RSH`）。
* 取反（`NEG`）。

1. **跳转指令** ：

* 无条件跳转（`JA`）。
* 条件跳转（`JEQ`、`JNE`、`JGT`、`JLT`、`JGE`、`JLE`）。
* 函数调用（`CALL`）和返回（`EXIT`）。

1. **辅助函数调用** ：

* 通过 `CALL` 指令调用内核提供的辅助函数（如 `bpf_map_lookup_elem`、`bpf_trace_printk`）。

1. **原子操作指令** ：

* 支持原子操作（如 `ATOMIC_ADD`、`ATOMIC_OR`），用于并发场景。

1. **32 位子寄存器操作** ：

* 支持 32 位子寄存器操作（如 `ADD32`、`SUB32`），减少类型扩展指令。

---

### 2. **BPF 寄存器**

BPF 虚拟机有 11 个通用寄存器（R0-R10）和一个程序计数器（PC）。寄存器的作用如下：

* **R0** ：用于存储函数返回值或操作结果。
* **R1-R5** ：用于传递函数参数。
* **R6-R9** ：通用寄存器，可用于临时存储数据。
* **R10** ：栈帧指针（只读），用于访问栈上的数据。

---

### 3. **BPF 内存模型**

BPF 的内存模型包括：

1. **栈** ：

* 每个 BPF 程序有一个固定大小的栈（通常为 512 字节），用于存储局部变量和临时数据。
* 通过 `R10` 寄存器访问栈。

1. **映射（Maps）** ：

* BPF 映射是一种键值对存储结构，用于在内核和用户空间之间共享数据。
* 支持多种映射类型（如哈希表、数组、环形缓冲区等）。

1. **上下文数据** ：

* BPF 程序可以访问特定的上下文数据（如网络数据包、跟踪事件等）。
* 上下文数据的格式由程序类型决定。

## LLVM 支持BPF

### 1. **BPF.td**

这是 BPF 目标架构的主定义文件，通常包含以下内容：

* 目标架构的基本描述（如寄存器、指令集、调用约定等）。
* 包含其他 `.td` 文件的引用。
* 定义目标架构的全局属性和配置。

 **作用** ：

* 作为 BPF 后端的主入口文件，整合所有相关的定义。
* 定义目标架构的全局特性。

---

### 2. **BPFInstrFormats.td**

该文件定义了 BPF 指令的格式。每条指令的编码格式（如操作码、寄存器、立即数等）都在此文件中定义。

 **作用** ：

* 定义 BPF 指令的二进制编码格式。
* 描述指令的字段布局（如操作码、源寄存器、目标寄存器、偏移量等）。

 **示例** ：

```
class InstBPF<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  bits<32> Inst;
  let Inst{31-24} = op;
  ...
}
```

---

### 3. **BPFInstrInfo.td**

该文件定义了 BPF 的具体指令。每条指令的操作码、操作数、汇编格式和编码都在此文件中定义。

 **作用** ：

* 定义 BPF 指令的具体实现。
* 将指令的操作码、操作数和汇编格式关联起来。

 **示例** ：

```
def ADD_rr : InstBPF<0x01, (outs GPR:$dst), (ins GPR:$src1, GPR:$src2),
                     "add $dst, $src1, $src2", []>;
```

---

### 4. **BPFRegisterInfo.td**

该文件定义了 BPF 的寄存器及其属性。包括通用寄存器、特殊寄存器（如栈指针、程序计数器）等。

 **作用** ：

* 定义 BPF 的寄存器集合。
* 描述寄存器的属性（如是否为保留寄存器、是否可用于某些操作等）。

 **示例** ：

```
def R0 : Register<"r0">;
def R1 : Register<"r1">;
...
def R10 : Register<"r10">;
```

---

### 5. **BPFRegisterBanks.td**

该文件定义了 BPF 的寄存器组（Register Banks）。寄存器组用于将寄存器分类，以便在指令选择时进行优化。

 **作用** ：

* 定义寄存器的分组（如通用寄存器组、浮点寄存器组等）。
* 优化指令选择时对寄存器的使用。

 **示例** ：

```
def GPRB : RegisterBank<"GPR", [R0, R1, ..., R10]>;
```

---

### 6. **BPFCallingConv.td**

该文件定义了 BPF 的调用约定（Calling Convention）。调用约定规定了函数调用时参数传递、返回值传递和寄存器使用的规则。

 **作用** ：

* 定义 BPF 的函数调用约定。
* 描述参数和返回值如何通过寄存器或栈传递。

 **示例** ：

```c++
def BPF_C : CallingConv<[
  // 参数传递规则
  CCIfType<[i32], CCAssignToReg<[R1, R2, R3, R4, R5]>>,
  // 返回值传递规则
  CCIfType<[i32], CCAssignToReg<[R0]>>
]>;
```

## LLVM添加一个新后端

由于llvm的结构化设计和实现，当为它添加新的后端是一般来书只需要对后端指令集、ABI(二进制接口接口)进行处理即可，但是现实情况是，一些后端会定义独有的数据类型、指令等，LLVM框架通常不能处理，此时需要添加特殊处理功能，否则会出错，此外，为了生成高质量的后端代码，LLVM还会针对后端经销一些特有的优化。

### 适配新后端的各个阶段

LLVM IR --------> ( 指令选择 )--------> 指令调度1 -------> 编译优化-1 --------> [ 寄存器分配 ] ----------> (插入前言/后序) ----

    -----> 编译优化-2--------> 指令调度-2---------> 其他优化 ------------>( 机器码生成)

（ ）[ ] 是添加新后端的必要过程 ，其他是优化是影响最后生成代码质量

## 指令选择

指令选择的工作是将LLVM IR 转换成目标相关的后端IR表达MachineInstr。

### 流程

LLVM IR -----> SelectionDAGBuilder ------{ SelectionDAG } ---->[ SelectionDAGsel | TargetLowering ] ------|

    ---------> { Machine SelectionDAG} --------> ScheduleDAGSDNode ------------> MachineInstr

从IR结构上区分，指令选择包含3个阶段：SelcctionDAG构建、Machine SelectionDAG匹配目标相关的指令操作和MachineInstr生成

### LLVM IR

LLVM IR 是 LLVM编译器的中间表示，他是一种与平台无关的、高级的伪汇编语言。LLVM IR 是编译器前端生成的中间结果，用于表示程序的逻辑，LLVM IR的设计目标是简洁、易于优化，并且能够高效地转换为目标机器代码。

### SelectionDAGBuilder

作用是将LLVM IR转换为一个有向无环图(DAG)结构，称为SelectionDAG,是一种中间表示，用于更直观地表示指令之间的依赖关系和操作。

### SelectionDAG

一个有向无环图，用于表示程序的指令流和数据依赖关系，每个节点(SDNode)表示一个操作(如加法、乘法、加载、存储等)，边表示依赖关系。与目标无关属性。

### TargetLowering 和 SelectionDAGSel

TargetLowering和SelectionDAGSel(DAG指令选择)是将目标无关的SelectionDAG转换的目标机器指令的过程。

* TargetLowering:
  作用： 将目标无关的SelectionDAG节点转换为目标架构支持的操作。例如，某些复杂的操作可能需要分解为多个目标架构支持的简单操作。
  输入：目标无关SelectionDAG
  输出：目标相关的 SelectionDAG (Machine SelectionDAG)
  主要任务：
  * 操作分解： 将复杂的操作分解为目标架构支持的简单操作
  * 指令选择： 根据目标架构的特性，选择合适的指令来实现每个操作
* SelectionDAGSel:
  作用 ：将目标相关的SelectionDAG转换为MachineInstr
  输入 ：目标相关的SelectionDAG
  输出：MachineInstr列表
  主要任务：
  * 指令匹配：根据目标架构的指令集，将SelectionDAG中的节点匹配到具体的机器指令
  * 指令生成：生成目标架构的机器指令

### Machine SelectionDAG

Machine SelectionDAG 是经过TargetLowering 处理后的目标相关的SelectionDAG。已经考虑了目标架构的属性，但是任为一个图结构，需要进一步转换为线性的机器指令序列。

### ScheduleDAGSDNode

ScheduleDAGDNode是对Mahcine SelectionDAG进行指令调度的阶段。指令调度的目的是优化指令的执行顺序，以提高目标机器的性能。

* 输入： Machine SelectionDAG
* 输出 ： 有序的MachineInstr 列表
* 主要任务：
  * 依赖分析：分析指令之间的依赖关系，确保指令的执行顺序符合依赖关系
  * 调度策略：根据目标架构的特性()
  * 指令排序：将SelectionDAG中的节点转换为线性的MachineInstr列表

### MachineInstr

MachineInstr是LLVM后端代码生成的最终结果，表示目标机器的指令，每个MachieInstr对应一条目标机器的指令。

* 输入：有序的MachineInstr列表
* 输出：目标机器代码
* 主要任务：

  * 寄存器分配：为指令中的变量分配目标机器的寄存器
  * 指令编码：将MachineInstr转换为二进制机器代码
  * 代码生成：生成最终的目标机器代码

### 总结

1. LLVM IR 被转换为目标无关的SelectionDAG
2. TargetLowering 将目标无关的 SelectionDAG转换为目标相关的Machine SelectionDAG
3. SelectionDAGSel 将目标相关的 Machine SelectionDAG 转换为MachineInstr
4. ScheduleDAGSDNode 对MachineInstr进行指令调度，生成有序的指令序列。
5. 最终MachineInstr被转换为目标机器代码
