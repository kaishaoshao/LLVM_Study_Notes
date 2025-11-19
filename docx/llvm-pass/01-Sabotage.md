# 😈 LLVM 入门实战：编写你的第一个 "Sabotage" Pass

### 🎯 目标

我们要写一个 LLVM 插件，它会自动寻找代码里的 **加法 (`+`)** ，把它们全改成 **减法 (`-`)** 。

### 📂 目录结构

请在桌面创建一个新文件夹 `llvm-sabotage`，并在里面创建三个文件：

1. `CMakeLists.txt` (构建说明书)
2. `Sabotage.cpp` (源代码)
3. `test.c` (测试代码)

---

### 第一步：编写构建脚本 (`CMakeLists.txt`)

CMake 是 C++ 界的“包工头”。这段代码告诉它：去哪里找 LLVM，以及如何把我们的代码打包成 `.so` 文件。

**CMake**

```
cmake_minimum_required(VERSION 3.13)
project(SabotagePlugin)

# 1. 自动在你的电脑里寻找 LLVM 安装路径
find_package(LLVM REQUIRED CONFIG)

# 2. 引入 LLVM 提供的辅助工具 (为了使用 add_llvm_pass_plugin 等功能)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

# 3. 设置头文件路径 (让编译器能找到 llvm/IR/PassManager.h 等)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

# 4. 生成插件
# "MODULE" 的意思是：生成一个动态库(.so/.dylib)，给 opt 工具加载用
add_library(SabotagePass MODULE Sabotage.cpp)

# Linux 下的兼容性补丁
if(UNIX AND NOT APPLE)
  target_link_libraries(SabotagePass PRIVATE LLVM)
endif()
```

---

### 第二步：编写核心逻辑 (`Sabotage.cpp`)

这是本教程的重点。请仔细阅读代码中的中文注释，我解释了每一个关键技巧。

**C++**

```
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

// [技巧1] PassInfoMixin: 
// 这是一个模板类。只要继承它，LLVM 就会自动帮你处理好 Pass 的身份证ID等繁琐细节。
struct SabotagePass : public PassInfoMixin<SabotagePass> {

  // [技巧2] run 函数: 
  // 这是 Pass 的主入口。每当 LLVM 处理到一个函数(Function F)时，都会调用它。
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    bool Changed = false;
  
    // 我们需要一个列表来暂存要修改的指令。
    // 为什么？因为如果一边遍历一边修改(删除)，迭代器会失效，程序会崩。
    std::vector<Instruction*> ToBeSabotaged;

    // --- 阶段 1: 扫描 (Scan) ---
    // 像剥洋葱一样：函数(Function) -> 基本块(BasicBlock) -> 指令(Instruction)
    for (auto &BB : F) {
      for (auto &I : BB) {
        // [技巧3] dyn_cast (动态转换):
        // 它的意思是："尝试把 I 转成二元运算符。如果成功返回指针，失败返回 nullptr"。
        // 只有当 I 确实是二元运算，且操作码是 Add (加法) 时，才进入 if。
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
          if (BinOp->getOpcode() == Instruction::Add) {
            ToBeSabotaged.push_back(&I);
          }
        }
      }
    }

    // --- 阶段 2: 破坏 (Modify) ---
    for (auto *Inst : ToBeSabotaged) {
      // [技巧4] IRBuilder (指令生成器):
      // "我想在 Inst 这条旧指令的旁边，插一条新指令"
      IRBuilder<> Builder(Inst);

      // 创建减法指令 (Sub)
      // 参数0和1取自旧指令 (例如 a + b，这里就取 a 和 b)
      Value *NewSub = Builder.CreateSub(Inst->getOperand(0), Inst->getOperand(1), "sabotage_sub");

      // [技巧5] RAUW (Replace All Uses With):
      // 这是最强的一招。意思是：
      // "LLVM，请帮我把后面所有用到 '旧加法结果' 的地方，自动改成用 '新减法结果'。"
      Inst->replaceAllUsesWith(NewSub);
    
      // 现在旧指令没人用了，把它删掉
      Inst->eraseFromParent();

      errs() << "😈 嘿嘿，把一个加法变成了减法!\n";
      Changed = true;
    }

    // [技巧6] PreservedAnalyses (结果汇报):
    // 如果 Changed 为真，返回 none() -> "我改了代码，之前的分析数据作废，请重算"。
    // 如果 Changed 为假，返回 all()  -> "我没动代码，之前的分析数据继续用"。
    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
  }
};

} // end anonymous namespace

// --- 插件注册 (这部分通常是样板代码，照抄即可) ---
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "SabotagePass", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          // 这里的 "sabotage" 决定了我们在命令行用什么名字调用它
          if (Name == "sabotage") {
            FPM.addPass(SabotagePass());
            return true;
          }
          return false;
        });
    }};
}
```

---

### 第三步：编写受害者代码 (`test.c`)

我们要写一个 C 程序来验证效果。

**C**

```
#include <stdio.h>

// 这个函数将被我们的 Pass 修改
int my_add(int a, int b) {
    return a + b;
}

int main() {
    // [新手坑] 为什么要用 volatile？
    // 如果不用，聪明的 Clang 编译器会直接算出 5+3=8，
    // 根本不会生成 "add" 指令，那我们的 Pass 就没东西可改了。
    volatile int x = 5;
    volatile int y = 3;
  
    printf("Expected: 8\n");
    // 如果 Pass 生效，这里应该输出 2 (5-3)
    printf("Result:   %d\n", my_add(x, y));
    return 0;
}
```

---

### 第四步：编译、运行与见证 (黄金流程)

请打开终端，严格执行以下 4 步。

#### 1. 编译插件

**Bash**

```
mkdir -p build && cd build
cmake ..
make
# 成功后，你会看到 SabotagePass.so (或者 .dylib)
```

#### 2. 生成 IR (避坑关键!)

回到项目根目录。我们要把 C 变成 LLVM IR。
**注意：** 必须加上 `-Xclang -disable-O0-optnone`。这个参数是为了防止编译器给函数打上“禁止优化”的标签（optnone），否则你的 Pass 会被跳过。

**Bash**

```
cd ..
clang -S -emit-llvm -O0 -Xclang -disable-O0-optnone test.c -o test.ll
```

#### 3. 运行你的 Pass

这是见证奇迹的时刻。加载插件，处理 `test.ll`。

**Bash**

```
# Linux 用户:
opt -load-pass-plugin=./build/SabotagePass.so -passes="sabotage" test.ll -S -o sabotaged.ll
```

*(如果是 macOS，把 .so 换成 .dylib)*

👀 **检查：** 终端必须打印出：`😈 嘿嘿，把一个加法变成了减法!`

#### 4. 执行结果

用 LLVM 的解释器运行修改后的代码：

**Bash**

```
lli sabotaged.ll
```

**预期输出：**

**Plaintext**

```
Expected: 8
Result:   2
```

---

### 🎓 新手必记的小抄 (Cheat Sheet)

在 LLVM 编程中，你以后会天天用到这三招：

1. **`dyn_cast<Type>(Ptr)`** :

* *人话：* "你是这个类型吗？是就转过去，不是就给空指针。" (比 C++ 原生转换更安全)。

1. **`builder.CreateXYZ(...)`** :

* *人话：* "在这里帮我插入一条 XYZ 指令。"

1. **`replaceAllUsesWith(NewVal)`** :

* *人话：* "把所有用旧值的地方，全部自动替换成用新值。" (省去了手动查找引用的麻烦)。

快去试试吧！如果你看到结果是  **2** ，恭喜你，你已经迈入了编译器开发的大门！
