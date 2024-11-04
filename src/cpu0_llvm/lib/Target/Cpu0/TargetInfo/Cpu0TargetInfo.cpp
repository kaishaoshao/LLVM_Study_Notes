//===-- Cpu0TargetInfo.cpp - Cpu0 Target Implementation -------------------===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Cpu0TargetInfo.h"
#include "../MCTargetDesc/Cpu0MCTargetDesc.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;
Target llvm::TheCpu0Target, llvm::TheCpu0elTarget;
extern "C" void LLVMInitializeCpu0TargetInfo() {
  RegisterTarget<Triple::cpu0,
                 /*HasJIT=*/true>
      X(TheCpu0Target, "cpu0", "CPU0 (32-bit big endian)", "Cpu0");
  RegisterTarget<Triple::cpu0el,
                 /*HasJIT=*/true>
      Y(TheCpu0elTarget, "cpu0el", "CPU0 (32-bit little endian)", "Cpu0");
}