# LLVM学习笔记

记录我学习LLVM的过程,前期是学习官方的教程从编写 `Kaleidoscope` 开始。下面来介绍一下官网的地址：[LLVM Tutorial](https://llvm.org/docs/tutorial/index.html#building-a-jit-in-llvm)
还有一个中文版 [用LLVM开发新语言](https://www.bookstack.cn/read/llvm-guide-zh/README.md)。我希望能够吧把官方推荐的教学资料全部学完。
(一段时间后)
目前暂时停止 `Kaleidoscope` 了，因为我想系统学习一下编译原理，找到一个不戳的学习博客[编译原理学习之路 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/713322939)，
推荐看中科大的《编译原理》配合学习完成[craftinginterpreters](https://github.com/GuoYaxiang/craftinginterpreters_zh?tab=readme-ov-file)
又发现一门好课，湖南大学的陈果老师的课，与科大的课相比与学生互动比较多。

新的学习资料 

[llvm使用入门--飞翼剑仆](https://blog.csdn.net/Zhanglin_Wu/article/details/124942823)
[现代编译器实习--龙芯--中科大](https://ustc-compiler-principles.github.io/textbook/#%E6%95%99%E6%9D%90%E4%BB%A3%E7%A0%81)  

学习llvm后端的项目

[LLVM_for_cpu0](https://github.com/P2Tree/LLVM_for_cpu0)

## 目录结构 && 文件

* 目录说明

```.
.
├── docx   ---> 参考资料
|   └── craftinginterpreters_zh(submodule) ----> 编译项目
└── src	   ---> 源码
    └── Kaleidoscope ---->Implementing a Language with LLVM

```

* 文件说明
  `git.sh` : 用于推送代码的脚本

  * 使用方式：`bash git.sh "test"`
* 教学视频

  [中科大-编译原理-华保健老师](https://www.bilibili.com/video/BV16h411X7JY/?spm_id_from=333.337.search-card.all.click&vd_source=2cb22062bbd1ad0823747ec35d88c863)

  [湖南大学计科拔尖班编译系统---陈果老师](https://www.bilibili.com/video/BV1FA4m1P7kn/?spm_id_from=333.788&vd_source=2cb22062bbd1ad0823747ec35d88c863)

  学习笔记

## llvm_for_cpu0

前置知识

* [llvm入门]([http://llvm.org/docs/GettingStarted.html\#id8](http://llvm.org/docs/GettingStarted.html#id8))
* [LLVM IR](https://evian-zhang.github.io/llvm-ir-tutorial/)
* [TableGen](https://zhuanlan.zhihu.com/p/141265959)
* [后端流程](https://zhuanlan.zhihu.com/p/301653651)

## craftinginterpreters

## LLVM Tutorial: Table of Contents

### Kaleidoscope: Implementing a Language with LLVM

### 工作原理
