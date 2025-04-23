//===-- BPF.td - Describe the BPF Target Machine -----------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// 包含 LLVM 目标描述框架的基类定义
include "llvm/Target/Target.td"

// 包含 BPF 特有的定义
include "BPFRegisterInfo.td"  // 寄存器定义（如 r0-r10）
include "BPFCallingConv.td"   // 调用约定（参数传递规则）
include "BPFInstrInfo.td"     // 指令集定义（如 ADD、LDW 等）

// 定义指令信息容器，将用于生成 BPFInstrInfo 类
def BPFInstrInfo : InstrInfo;

// 定义处理器模型（用于子目标特性配置）
// Name: 处理器名称
// Features: 支持的子目标特性列表
class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>; // NoItineraries 表示无调度信息

// 具体处理器定义（BPF 的不同版本）
def : Proc<"generic", []>;   // 通用基础版本
def : Proc<"v1", []>;        // v1 版本
def : Proc<"v2", []>;        // v2 版本
def : Proc<"v3", []>;        // v3 版本
def : Proc<"probe", []>;     // 探测用特殊版本

// 子目标特性定义（编译时可启用的功能）
def DummyFeature : SubtargetFeature<"dummy", "isDummyMode",
                                    "true", "unused feature">; // 示例用的空特性

def ALU32 : SubtargetFeature<"alu32", "HasAlu32", "true",
                             "Enable ALU32 instructions">; // 启用 32 位 ALU 指令

def DwarfRIS: SubtargetFeature<"dwarfris", "UseDwarfRIS", "true",
                               "Disable MCAsmInfo DwarfUsesRelocationsAcrossSections">; // 调试信息相关设置

// 汇编代码生成器配置
def BPFInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter"; // 使用 InstPrinter 类生成汇编
  bit isMCAsmWriter = 1;                      // 标记为 MC 层汇编生成器
}

// 汇编解析器配置
def BPFAsmParser : AsmParser {
  bit HasMnemonicFirst = 0; // 指令不以助记符开头（BPF 汇编的特殊语法）
}

// 汇编方言配置
def BPFAsmParserVariant : AsmParserVariant {
  int Variant = 0;                   // 方言版本号
  string Name = "BPF";               // 方言名称
  string BreakCharacters = ".";      // 分隔符字符
  string TokenizingCharacters = "#()[]=:.<>!+*"; // 特殊词法分析字符
}

// 目标机器整体定义
def BPF : Target {
  let InstructionSet = BPFInstrInfo;          // 绑定指令集
  let AssemblyWriters = [BPFInstPrinter];     // 指定汇编生成器
  let AssemblyParsers = [BPFAsmParser];       // 指定汇编解析器
  let AssemblyParserVariants = [BPFAsmParserVariant]; // 指定方言配置
}