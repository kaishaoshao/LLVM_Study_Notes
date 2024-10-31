# llvm

## 目录结构

### **benchmarks**

包含性能基准测试工具和测试套件，用于评估 LLVM 编译器和相关工具的性能。

### **bindings**

包含 LLVM 的语言绑定，允许其他编程语言（如 Python、Java 等）与 LLVM 交互。

### **cmake**

包含 CMake 构建系统的相关文件，用于配置和生成 LLVM 项目的构建文件。

### **configure**

* 自动配置脚本，用于检测系统环境并生成配置文件（如 `config.h`），以便 LLVM 能够适应不同的系统和平台。

### **docs**

* 包含 LLVM 的文档，包括用户指南、开发者文档、API 文档等。

### **examples**

提供了一些示例程序，展示如何使用 LLVM 的各种组件。

* **BrainF**

  * 一个简单的 Brainfuck 语言解释器，使用 LLVM 作为后端来生成机器代码
* **Bye**

  * 一个简单的“Hello World”级别的示例，展示如何使用 LLVM 来生成一个简单的“再见”消息。
* **ExceptionDemo**

  * 一个演示如何在 LLVM 中处理异常（exception handling）的示例。
* **Fibonacci**

  * 一个使用 LLVM 来生成 Fibonacci 数列计算代码的示例。
* **HowToUseJIT**

  * 一个教程式的示例，展示如何使用 LLVM 的即时编译（JIT）功能。
* **HowToUseLLJIT**

  * 展示如何使用 LLVM 的 LLJIT 组件，这是 LLVM 新一代的 JIT 编译器。
* **IRTransforms**

  * 一个展示如何在 LLVM 中进行中间表示（IR）转换的示例。
* **Kaleidoscope**

  * 一个系列教程，从零开始构建一个简单的编译器，支持一个名为 Kaleidoscope 的玩具语言。
* **ModuleMaker**

  * 一个示例，展示如何使用 LLVM 的模块构建器（ModuleMaker）来创建和操作 LLVM 模块。
* **OrcV2Examples**

  * 包含使用 LLVM ORC（On-Request Compilation）V2 API 的示例，展示如何进行动态编译和链接。
* **ParallelJIT**

  * 一个示例，展示如何使用 LLVM 的并行 JIT 功能。
* **SpeculativeJIT**
* 一个示例，展示如何在 LLVM 中实现推测性 JIT 编译

### **lib**

包含 LLVM 的库文件，这些库文件实现了 LLVM 的核心功能。

### **projects**

包含 LLVM 项目的一些子项目，这些子项目与 LLVM 紧密相关，但可能作为独立的模块进行开发。

### **runtimes**

包含 LLVM 的运行时库，如编译器运行时（libgcc）的替代品。

### **test**

包含 LLVM 的测试套件，用于验证 LLVM 的功能和稳定性。

### **tools**

包含 LLVM 的工具链，如 `clang`、`lldb`、`llc`、`opt` 等。

### **unittests**

包含 LLVM 的单元测试，用于测试 LLVM 组件的各个部分。

### **utils**

包含 LLVM 的一些辅助工具和脚本，用于构建过程、代码分析等。
