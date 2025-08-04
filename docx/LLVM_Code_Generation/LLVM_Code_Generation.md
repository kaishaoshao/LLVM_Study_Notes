# LLVM_Code_Generation

## ch1

### 使用clang 实验

| To Stop                                  | Command                                   | 验证                                        |
| ---------------------------------------- | ----------------------------------------- | ------------------------------------------- |
| 预处理之后                               | clang -E                                  | clang -E clang -E ./ch1/helloworlf.c       |
| 语法检查后                               | clang -fsyntax-only                       | clang -fsyntax-only ./ch1/helloworlf_err.c |
| LLVM IR 代码生成后                       | clang -O0 -emit-llvm -S                   | clang -O0 -emit-llvm -S ./ch1/pow.c         |
| 中端优化后(选择你想要的级别)             | clang -O<1 / 2 / 3 / s / z> -emit-llvm -S | clang -Os -emit-llvm -S ./ch1/pow.c         |
| 汇编生成后<br />())                      | clang -S                                  | clang -S ./ch1/pow.c                       |
| 汇编器之后<br />(i.e,see the objdect fi) | clang -c                                  | clang -c ./ch1/pow.c                        |

核心LLVM项目

| 工具    | 作用                                                                         |
| ------- | ---------------------------------------------------------------------------- |
| opt     | 构建用于LLVM IR 到 LLVM IR 优化的驱动程序                                    |
| llc     | 构建用于LLVM IR 到汇编 / 目标文件管道的驱动程序                              |
| llvm-mc | 构建用于汇编/ 反汇编机制交互的工具                                           |
| check   | 运行所有核心LLVM单元测试,此目标将自动重建参与单元测试的所有工具,包括上述目标 |

### llvm测试工具 lit



## ch2 Contributing to LLVM

1. Fork the LLVM repository:
   1.Go to https://github.com/llvm/llvm-project.
   2.Click on Fork on the right in the top banner.
   3.Click Create fork on the next page.
2. Clone either the main repository or your fork

   ```
   git clone https://github.com/llvm/llvm‑project.git
   ```
3. Make your changes.
4. Rebase the main branch from the open source repository
5. Push your changes in your fork.

   ```
   git rebase https://github.com/llvm/llvm-project.git main
   ```
6. Create a pull request to the main repository

   1.Go to https://github.com/llvm/llvm-project.1
   2.Click on Pull requests in the top bar
   3.Click on New pull request
   4.Click on compare across forks.
   5.Choose the official LLVM repository as the base repository and main as the base.
   6.Choose your fork as the head repository and myChange as the compare.6
   7.Click Create pull request.

## ch3 Compiler Basics and  How They Map to LLVM APIs
