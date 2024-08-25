# LLVM学习笔记

    记录我学习LLVM的过程,前期是学习官方的教程从编写`Kaleidoscope` 开始。下面来介绍一下官网的地址：[LLVM Tutorial](https://llvm.org/docs/tutorial/index.html#building-a-jit-in-llvm) 还有一个中文版，遗憾的是没有翻译完：[用LLVM开发新语言](https://llvm-tutorial-cn.readthedocs.io/en/latest/)。我希望能够吧把官方推荐的教学资料全部学完。

## 目录结构 && 文件

* 目录说明

```.
.
├── docx   ---> 参考资料
└── src	   ---> 源码
    └── Kaleidoscope ---->Implementing a Language with LLVM

```

* 文件说明
  `git.sh` : 用于推送代码的脚本
  * 使用方式：`bash git.sh "test"`


## LLVM Tutorial: Table of Contents

### Kaleidoscope: Implementing a Language with LLVM

### 工作原理
