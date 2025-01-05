# CPU0 笔记

## **Cpu0 架构概述**

Cpu0 是一个简化的 32 位 RISC 架构，主要用于教学和研究。其特点包括：

* 16 个通用寄存器（R0-R15）和特殊寄存器（如 PC、LR、SP 等）。
* 指令集分为 L 型（内存操作）、A 型（算术操作）和 J 型（控制流操作）。[Cpu0 架构](https://cpu0-llvm17-learning.readthedocs.io/en/latest/llvmstructure_zh.html)
* 支持基本的整数运算、内存访问和控制流指令。

## **LLVM 后端开发流程**

接入 Cpu0 架构的核心步骤包括：

### **2.1 目标描述文件（.td 文件）**

LLVM 使用 TableGen 工具生成目标描述文件，主要包括：

* **Cpu0.td** ：定义目标架构的基本信息，如寄存器、指令集和调度策略。
* **Cpu0InstrInfo.td** ：定义指令及其匹配模式，例如 `ADDiu` 指令的格式和操作数。
* **Cpu0RegisterInfo.td** ：定义寄存器及其分类，如通用寄存器 `CPURegs` 和特殊寄存器 `SR`。
* **Cpu0Schedule.td** ：定义指令调度策略，优化指令执行顺序。

### **2.2 目标机器描述**

* **Cpu0TargetMachine.cpp** ：定义目标机器的接口，包括目标特性和优化选项**17**。
* **Cpu0Subtarget.cpp** ：定义目标子特性，如指令集扩展和硬件特性。

### **2.3 指令选择与 DAG 转换**

* **Cpu0ISelDAGToDAG.cpp** ：实现从 LLVM IR 到机器指令的转换，使用 DAG（有向无环图）进行模式匹配。
* **Cpu0ISelLowering.cpp** ：处理 LLVM IR 到目标架构的 Lowering 操作，例如函数调用和返回指令的处理。

### **2.4 汇编与反汇编**

* **Cpu0AsmPrinter.cpp** ：将机器指令转换为汇编代码**17**。
* **Cpu0Disassembler.cpp** ：实现反汇编功能，将机器码转换为汇编指令。

### **2.5 目标注册**

* **Cpu0TargetInfo.cpp** ：注册目标架构，使 LLVM 能够识别 Cpu0。
* **Cpu0MCTargetDesc.cpp** ：描述目标架构的机器代码生成细节，包括重定位和符号处理。

---

## **关键技术与实现细节**

### **3.1 指令定义与匹配**

在 `Cpu0InstrInfo.td` 中，通过 TableGen 定义指令及其匹配模式。例如：

tablegen

复制

```
def ADDiu : ArighLogicI<0x09, "addiu", add, simm16, immSExt16, CPURegs> {
  let isReMaterializable = 1;
}
```

这里定义了 `ADDiu` 指令，包括操作码、汇编字符串和匹配模式。

### **3.2 寄存器分配**

在 `Cpu0RegisterInfo.td` 中定义寄存器及其分类，例如：

```cpp
def CPURegs : RegisterClass<"Cpu0", [i32], 32, (add ZERO, AT, V0, V1, ...)>;
```

通过寄存器分配算法，将 LLVM IR 中的虚拟寄存器映射到物理寄存器。

### **3.3 指令调度**

在 `Cpu0Schedule.td` 中定义指令调度策略，优化指令执行顺序以适应处理器的流水线特性。

### **3.4 函数调用与返回**

在 `Cpu0ISelLowering.cpp` 中处理函数调用和返回指令，例如：

```
SDValue Cpu0TargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv, ...) {
  // 处理返回指令
}
```

确保函数调用和返回符合 Cpu0 的 ABI 规范。

---

## **调试与测试**

* 使用 `llc` 工具生成目标架构的汇编代码，验证指令选择和代码生成是否正确。
* 通过调试工具（如 GDB）跟踪后端代码的执行过程，定位和修复问题。

---

## **优化与扩展**

* **指令集扩展** ：根据需求添加新的指令，例如浮点运算或 SIMD 指令。
* **性能优化** ：通过指令调度和寄存器分配优化代码性能。
* **调试支持** ：添加调试信息生成功能，支持 GDB 等调试工具。

---

## 资料

* [LLVM后端实践笔记](https://github.com/P2Tree/LLVM_for_cpu0/)
* [LLVM Cpu0 新后端1](https://blog.csdn.net/lml435035844/article/details/139251202)
* [LLVM Cpu0 新后端 系列课程总结](https://blog.csdn.net/lml435035844/article/details/139560889)
* [iiicp-llvm20-cpu0](https://github.com/iiicp/llvm-cpu0)
* [LLVM CPU0 第三章 后端](https://blog.csdn.net/jinweifu/article/details/98440047)
* [Cpu0_for_LLVM17](https://github.com/HLRJ/Cpu0_For_LLVM17/blob/main/README.md)
* [cpu0-llvm17-learning](https://github.com/Laity000/cpu0-llvm17-learning/blob/main/lbd/zh_CN_docs/llvmstructure_zh.rst)
* [Cpu0后端初始化](https://blog.csdn.net/qiusi0225/article/details/123198584)
