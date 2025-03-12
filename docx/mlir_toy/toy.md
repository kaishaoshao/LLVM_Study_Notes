# Toy

## ch1

### 1. **词法分析器（Lexer）**

**作用**：将源代码转换为 Token 流。

- **关键函数**：
  - `getNextToken()`: 逐个字符读取输入，生成 Token（如标识符、数字、操作符等）。
  - `Token` 类型：包括 `identifier`、`number`、`keyword`（如 `def`、`var`）、`paren`（`(`、`)`）等。

**步骤**：

- 跳过空白字符，识别数字、标识符和符号。
- 例如输入 `def multiply(a, b) { return a * b }` 会被拆解为一系列 Token。

---

### 2. **语法分析器（Parser）**

**作用**：将 Token 流转换为抽象语法树（AST）。

- **关键函数**：
  - `parseModule()`: 解析整个程序（一组函数和变量声明）。
  - `parseFunction()`: 解析函数定义（如 `def multiply(...)`）。
  - `parseExpression()`: 解析表达式（如算术运算 `a * b`）。

**步骤**：

- 递归下降解析，按优先级处理表达式（如乘法优先于加法）。
- 例如 `a * b + c` 会被解析为 `BinOp(+, BinOp(*, a, b), c)`。

---

### 3. **AST 节点定义**

**作用**：表示程序的层次结构。

- **关键类**：
  - `FunctionAST`: 函数定义（包含函数名、参数列表、函数体）。
  - `VarDeclExprAST`: 变量声明（如 `var a = 5`）。
  - `BinaryExprAST`: 二元运算（如 `a + b`）。

**示例**：

```cpp
// 函数 multiply 的 AST 表示
FunctionAST(
  name: "multiply",
  args: ["a", "b"],
  body: ReturnStmt(BinaryExprAST('*', VariableExpr("a"), VariableExpr("b")))
```

---

### 4. **MLIR 生成（MLIRGen）**

**作用**：将 AST 转换为 MLIR 方言（`toy` Dialect）。

- **关键函数**：
  - `mlirGen(FunctionAST)`: 生成函数的 MLIR 表示。
  - `mlirGen(ExprAST)`: 递归生成表达式的 MLIR 操作。

**步骤**：

1. **创建 MLIR 上下文**：初始化 `mlir::MLIRContext`。
2. **构建模块**：`mlir::ModuleOp::create()`。
3. **生成函数原型**：
   ```mlir
   func.func @multiply(%a: tensor<*xf64>, %b: tensor<*xf64>) -> tensor<*xf64>
   ```
4. **生成函数体**：
   ```mlir
   %0 = toy.mul %a, %b : tensor<*xf64>
   toy.return %0 : tensor<*xf64>
   ```

---

### 5. **Toy Dialect 定义**

**作用**：定义 Toy 语言在 MLIR 中的操作和类型。

- **关键组件**：
  - `Toy_Dialect`: 注册方言和操作（如 `toy.mul`、`toy.return`）。
  - `Toy_Type`: 表示 Tensor 类型（如 `tensor<*xf64>`）。

**操作定义示例**：

```cpp
// 定义乘法操作
class MulOp : public mlir::Op<MulOp, mlir::OpTrait::NOperands<2>::Impl> {
  // 输入两个 tensor，输出一个 tensor
  static void build(..., mlir::Value lhs, mlir::Value rhs) { ... }
}
```

---

### 6. **验证和输出**

**作用**：确保生成的 MLIR 有效并打印结果。

- **关键函数**：
  - `module.verify()`: 检查 IR 的合法性。
  - `module.dump()`: 打印生成的 MLIR 代码。

**输出示例**：

```mlir
module {
  func.func @multiply(%a: tensor<*xf64>, %b: tensor<*xf64>) -> tensor<*xf64> {
    %0 = toy.mul %a, %b : tensor<*xf64>
    toy.return %0 : tensor<*xf64>
  }
}
```

---

### 总结流程

```
源代码 → Lexer → Token流 → Parser → AST → MLIRGen → MLIR 方言
```

每个步骤通过函数逐级转换，最终生成 MLIR 表示。第 1 章重点在于前端到 MLIR 的基础映射，后续章节会引入优化和代码生成。
