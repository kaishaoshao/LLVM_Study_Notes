//===-- BPF.td - Describe the BPF Target Machine -----------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

include "BPFRegisterInfo.td"
include "BPFCallingConv.td"
include "BPFInstrInfo.td"
include "GISel/BPFRegisterBanks.td"

def BPFInstrInfo : InstrInfo;

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def DummyFeature : SubtargetFeature<"dummy", "isDummyMode",
                                    "true", "unused feature">;

def ALU32 : SubtargetFeature<"alu32", "HasAlu32", "true",
                             "Enable ALU32 instructions">;

def DwarfRIS: SubtargetFeature<"dwarfris", "UseDwarfRIS", "true",
                               "Disable MCAsmInfo DwarfUsesRelocationsAcrossSections">;

def : Proc<"generic", []>;
def : Proc<"v1", []>;
def : Proc<"v2", []>;
def : Proc<"v3", [ALU32]>;
def : Proc<"v4", [ALU32]>;
def : Proc<"probe", []>;

def BPFInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

def BPFAsmParser : AsmParser {
  bit HasMnemonicFirst = 0;
}

def BPFAsmParserVariant : AsmParserVariant {
  int Variant = 0;
  string Name = "BPF";
  string BreakCharacters = ".";
  string TokenizingCharacters = "#()[]=:.<>!+*%/";
}

def BPF : Target {
  let InstructionSet = BPFInstrInfo;
  let AssemblyWriters = [BPFInstPrinter];
  let AssemblyParsers = [BPFAsmParser];
  let AssemblyParserVariants = [BPFAsmParserVariant];
}


#ifndef __RISCV_CSR_H
#define __RISCV_CSR_H

#ifndef __RISCV_XLEN
#ifndef __riscv_xlen
#define __RISCV_XLEN 32
#else
#define __RISCV_XLEN __riscv_xlen
#endif
#endif /* __RISCV_XLEN */

#if __RISCV_XLEN == 32
typedef uint32_t rv_csr_t;
#elif __RISCV_XLEN == 64
typedef uint64_t rv_csr_t;
#else
typedef uint32_t rv_csr_t;
#endif
#define __STR(s) #s
#define STRINGIFY(s) __STR(s)

#define __RV_CSR_READ(csr)                                                     \
  ({                                                                           \
    register rv_csr_t __v;                                                     \
    __asm volatile("csrr %0, " STRINGIFY(csr) : "=r"(__v) : : "memory");       \
    __v;                                                                       \
  })

static inline uint64_t __get_rv_instret(void) {
#if __RISCV_XLEN == 32
  volatile uint32_t high0, low, high;
  uint64_t full;

  high0 = __RV_CSR_READ(CSR_MINSTRETH);
  low = __RV_CSR_READ(CSR_MINSTRET);
  high = __RV_CSR_READ(CSR_MINSTRETH);
  if (high0 != high) {
    low = __RV_CSR_READ(CSR_MINSTRET);
  }
  full = (((uint64_t)high) << 32) | low;
  return full;
#elif __RISCV_XLEN == 64
  return (uint64_t)__RV_CSR_READ(CSR_MINSTRET);
#else // TODO Need cover for XLEN=128 case in future
  return (uint64_t)__RV_CSR_READ(CSR_MINSTRET);
#endif
}

static inline uint64_t __get_rv_cycle(void) {
#if __RISCV_XLEN == 32
  volatile uint32_t high0, low, high;
  uint64_t full;

  high0 = __RV_CSR_READ(CSR_MCYCLEH);
  low = __RV_CSR_READ(CSR_MCYCLE);
  high = __RV_CSR_READ(CSR_MCYCLEH);
  if (high0 != high) {
    low = __RV_CSR_READ(CSR_MCYCLE);
  }
  full = (((uint64_t)high) << 32) | low;
  return full;
#elif __RISCV_XLEN == 64
  return (uint64_t)__RV_CSR_READ(CSR_MCYCLE);
#else // TODO Need cover for XLEN=128 case in future
  return (uint64_t)__RV_CSR_READ(CSR_MCYCLE);
#endif
}
#endif


/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include "hpm_csr_regs.h"
#include "hpm_debug_console.h"
#include <stdio.h>

#include "csr.h"

#define LED_FLASH_PERIOD_IN_MS 300

int run_test(void);

int main(void)
{
    board_init();
    board_init_led_pins();

    board_timer_create(LED_FLASH_PERIOD_IN_MS, board_led_toggle);
    unsigned long begin_instret, end_instret, instret_nice;
    unsigned long begin_cycle, end_cycle, cycle_nice;
    begin_instret = __get_rv_instret();
    begin_cycle = __get_rv_cycle();

    int ret = run_test();

    end_instret = __get_rv_instret();
    end_cycle = __get_rv_cycle();

    instret_nice = end_instret - begin_instret;
    cycle_nice = end_cycle - begin_cycle;
    printf("mcycle: %lu, icount: %lu\n", cycle_nice, instret_nice);
    return ret;
}
