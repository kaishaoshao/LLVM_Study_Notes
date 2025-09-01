# MLIR

相关资料
[Stephen Diehl](https://www.stephendiehl.com/posts/mlir_introduction)
[Stephen Diehl-github](https://github.com/sdiehl/mlir-egglog)
[知乎-见南山](https://zhuanlan.zhihu.com/p/1905706607217714443)

## Part1 Introduction MLIR

通过精心设计的C/python program直接交互的MLIR代码实例，对Tensor\Memref\Affine\Linalg等关键Dialect的核心概念进行解析
目标： 最终实现一个精简版本的GPT2 transformer模型，并将其编译为高效的GPU计算内核

### 命名约定

* `%` 前缀: [SSA 值]()(例如 `%result`)
* `@` 前缀: Functions (例如 `@fun`)
* `^` 前缀: [Basic blocks]() (例如 `^bb0`)
* `#` 前缀: Attribute aliases (例如 `#map_1d_identity`)
* `x` 分隔符: 在shape信息中用来分隔维度与类型 (e.g. `10xf32`)
* `:` and `->`用于表示操作或值的类型（例如 %result: i32）
* `!` 前缀: 类型别名（例如 !avx_m128 = vector<4 x f32>）
* `()` 用于表示参数（例如 (%arg0, %arg1)）
* `{}` 用于表示区域（regions）
* `//` 注释
* `<>` 用于表示类型参数（例如 tensor<10xf32>）

### 基本概念

* **Modules** : 模块，组顶层的MLIR操作的容器。
  module { // Operations }
* **Functions** : 按特定顺序执行的操作集合。
  func.func @my_function(%arg0: i32, %arg1: i32) -> i32 { // Operations }
* **Dialects** : MLIR中领域特定的操作和类型集合
  一些常用的高级（high-level）方言:
* tensor: 提供张量（tensor）的创建与操作运算，支持以无副作用（side-effect-free）方式实现高级数学表达式与转换。
* linalg: 专为线性代数计算设计的操作集合，可高效实现矩阵与向量运算。 （译者按：linalg在MLIR中的功能远不止表示线性代数--linear algebra运算，它在高层级提供了AI软件算法常用高级运算的结构化表达，并支持对性能非常关键的tiling和fusion等优化。同时支持用户自定义扩展特定算法。）
* omp: 包含支持并行编程模型的操作，开发者可通过兼容OpenMP标准的方式表示并行逻辑。
* affine: 提供仿射运算与分析的表达框架，利用仿射表达式的可预测性实现优化。
* gpu: 面向GPU编程的专用操作集，支持异构架构上的并行执行。

常用的低级（lower-level）方言有：

* scf: 提供结构化控制流（structured control flow）比如循环和条件分支的操作。
* func: 包含函数定义与调用相关操作。
* memref: 专用于内存引用操作，可高效管理和操作内存缓冲区，对性能敏感型应用至关重要。
* index: 用来处理索引（index）的计算操作。Index 被用来对array和tensor中的元素寻址。比如在循环迭代中使用index访问数据元素。
* arith: 包含对整型和浮点型标量（scalars）、向量（vector）和张量（tensor）的基础数学运算、位运算和比较运算等操作

本系列教程后续也会用到针对NVidia GPU特定硬件的方言。

* **Operations** : MLIR 中的操作（operation）是最基本的工作单元，类似于 LLVM 的指令，但会额外包含**方言命名空间**和可能的 **类型说明** 。

```text
// MLIR 操作语法示例
%0 = "my_dialect.my_operation"(%arg0, %arg1) : (i32, i32) -> i32 

// my_dialect.my_operation：定义在 my_dialect 方言中的操作
// %arg0, %arg1：类型为 i32 的输入参数
// 返回值：类型为 i32，绑定到变量 %0
```

** **`arith.addf` 操作的例子：

```text
// 两个 f32 浮点数相加
%0 = arith.addf %arg0, %arg1 : f32   
```

* **Basic Blocks** : 基本块是一组顺序执行的操作序列，只有一个入口和一个出口，没有分支。它必须以终止操作（terminator operations）结束，比如cf.br、cf.cond_br或者return。
  它们用于组织程序控制流，便于编译器优化（如内联、循环展开）。
  基本块必须定义在函数内部，可通过不同方言（如 `scf`、`cf`）实现复杂控制流。

```text
^bb1:  // "then" 块的标签
 %then_result = arith.muli %result, 2 : i32  // 乘法操作
 return %then_result : i32                    // 退出基本块 
```

与LLVM 不同，MLIR 的基本块支持参数传递：^bb1(%result : i32)。

* **Regions** : 区域（Regions）通过 `{}` 对代码分组（group operations together），用来表示循环和条件语句等控制流结构。

```text
{
  ^bb1(%result : i32):
  %then_result = arith.muli %result, 2 : i32
  return %then_result : i32
}
```

* **Types** : 类型（Type）用来限定变量可存储的值的种类以及可对其执行的操作。类型对数据添加约束，确保操作仅在类型兼容的值上执行。

```text
%result = arith.constant 1 : i32
```

为方便起见，也可以为类型定义别名。

```text
!avx_m128 = vector<4 x f32>
```

* **Passes** : 是作用于MLIR方言（dialects）的转换（transformations），通过优化并将其降阶（lowering）为更简单的结构。
  下面列出的是MLIR中常用的一些pass，他们可以作为mlir-opt的参数，对MLIR实施转换。
  这些是可用于传递给mlir-opt以转换MLIR的参数，最常见的包括
* `convert-func-to-llvm`: 将类函数操作转换为LLVM方言
* `convert-math-to-llvm`: 将math运算转换为LLVM方言
* `convert-index-to-llvm`: 将index操作转换为LLVM方言
* `convert-scf-to-cf`: 将结构化控制流转换为CF方言
* `convert-cf-to-llvm`: 将控制流转换为LLVM方言
* `convert-arith-to-llvm`: 将arith操作转换为LLVM方言
* `reconcile-unrealized-casts`: 调和未实现的类型转换
* `convert-memref-to-llvm`: 将memref操作转换为LLVM方言
* `convert-tensor-to-llvm`: 将tensor操作转换为LLVM方言
* `convert-linalg-to-scf`: 将linalg操作转换为 `scf.for` 循环
* `convert-linalg-to-affine-loops`: 将linalg操作转换为 `affine.for` 循环
* `convert-omp-to-llvm`: 将OpenMP操作转换为LLVM方言
* `convert-vector-to-llvm`: 将vector操作转换为LLVM方言

此外还存在一个通用的 `-convert-to-llvm` pass 能尽力将所有MLIR转换到LLVM IR。实践中，我们会用更细粒度的pass进行MLIR到LLVM的转换。

还有一种更精细的转换pass粒度控制方法，是在   --pass-pipeline 的参数字符串中以逗号分隔列表的形式指定passes名称，mlir-opt会构造复杂的pass流水线来执行passes，完成转换。例如： --pass-pipeline="builtin.module(pass1,pass2)"

mlir-opt还支持多个方便debug passes的参数：

--mlir-print-ir-after-all：在每个通道后打印 IR

--mlir-print-ir-after-change：仅在 IR 变更后打印

--mlir-print-ir-after-failure：仅在转换失败后打印

--mlir-print-ir-tree-dir: 可以将print-ir的打印内容，写入到目录树中的文件中，而不是打印到stdout

**注意**：pass的顺序至关重要，例如 **convert-scf-to-cf** 必须位于 **convert-cf-to-llvm** 之前执行。


lowering 到 LLVM Dialect

LLVM IR采用静态单赋值（Static Single Assignment, SSA）形式，即每个变量只能被赋值一次，且所有对变量的使用都必须在其定义之后。在SSA中，每次"赋值"都会生成变量的新版本（通常用数字后缀表示，例如%1、%2等），而非覆盖原有变量。

采用SSA让编译器更容易判断变量的定义与使用之间的关系，做出更简单高效的优化。

比如在LLVM IR中，可能出现类似下面的代码：

```text
%1 = add i32 %a, %b
%2 = mul i32 %1, %c
```

例子中的%1和%2是SSA值，不可被重复赋值。

对于由于控制流语句（比如if...then...else）导致变量在不同路径可能有不同值时，LLVM会使用特殊的"phi" node来合并这些可能的值，从而在保持SSA特性的同时处理动态程序行为。

基本块（basic block）是LLVM控制流的基本单位，它是一组顺序执行的指令序列，有唯一的入口和出口。

[Phi node](https://zhida.zhihu.com/search?content_id=257692076&content_type=Article&match_order=1&q=Phi+node&zhida_source=entity)是SSA形式中间表示中的特殊指令，用于合并来自不同控制流路径的多个可能值。它们本质上是基于控制流的多路选择器，会根据程序实际执行的路径来选取正确的值。


## MLIR的标准Dialects

MLIR中的 `llvm` dialect 直接对应 LLVM IR. 它是MLIR层级中的最底层，可以直接被转换为LLVM IR。

### `scf` and `cf`

所有high-level的scf结构，都会被编译为cf操作

`cf.br` - 无条件跳转到basic block

```text
// A basic block
^bb0:
  cf.br ^bb1
```

`cf.cond_br` - 条件跳转到basic block

```text
// A conditional branch to a basic block
^bb0:
  cf.cond_br %c, ^bb1, ^bb2
```

`cf.switch` - 多路分支跳转

```text
// A switch to a basic block
^bb0:
  cf.switch %i, ^bb1, ^bb2, ^bb3
```

`scf.if` - scf条件语句

```text
scf.if %b  {
  // true region
} else {
  // false region
}
```

`scf.loop` - scf循环语句

```text
%lb = index.constant 0
%ub = index.constant 10
%step = index.constant 1

scf.for %iv = %lb to %ub step %step {
  // loop region
}
```

使用 `convert-scf-to-cf` pass 可以将scf 方言的操作lower到cf方言的操作。

### `arith` and `math`

`arith` dialect 包含这些基本操作:

* 整型算数运算: `addi`, `subi`, `muli`, `divsi` (signed), `divui` (unsigned)
* 浮点型算数运算: `addf`, `subf`, `mulf`, `divf`
* 比较运算: `cmpi` (integer), `cmpf` (float)
* 类型转换: `extsi` (sign extend), `extui` (zero extend), `trunci`, `fptoui`, `fptosi`, `uitofp`, `sitofp`
* 位操作: `andi`, `ori`, `xori`

算数运算的例子:

```text
func.func @arithmetic_example(%a: i32, %b: f32) -> i32 {
    // Integer addition
    %1 = arith.constant 42 : i32
    %2 = arith.addi %a, %1 : i32

    // Float to integer conversion
    %3 = arith.fptosi %b : f32 to i32

    // Final addition
    %4 = arith.addi %2, %3 : i32
    return %4 : i32
}
```

`math` dialect 提供更复杂的数学运算:

* Trigonometric: `sin`, `cos`, `tan`
* Exponential: `exp`, `exp2`, `log`, `log2`, `log10`
* Power functions: `pow`, `sqrt`
* Special functions: `erf`, `atan2`
* Constants: `constant`

Example using math operations:

```text
func.func @math_example(%x: f32) -> f32 {
    // Calculate sin(x) * sqrt(x)
    %1 = math.sin %x : f32
    %2 = math.sqrt %x : f32
    %3 = arith.mulf %1, %2 : f32
    return %3 : f32
}
```

使用 `-convert-math-to-llvm` 和 `-convert-arith-to-llvm` pass可以将math 和 arith方言的操作转换为LLVM方言操作。

### `index`

`index` 方言用于处理索引计算。MLIR中的index类型是一种与平台相关的有符号整数。它被用于内存寻址（比如索引memref和tensor中的元素）和作为循环变量。

Index操作有:

`index.constant`: Create an index constant

* `index.add`: Add two indices
* `index.sub`: Subtract two indices
* `index.mul`: Multiply two indices
* `index.cmp`: Compare two indices
* `index.divs`: Signed division of indices
* `index.rems`: Signed remainder of indices

使用 `-convert-index-to-llvm` pass可以将index方言操作转换为LLVM方言操作。
