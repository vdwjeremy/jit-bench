
/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * SPDX-License-Identifier: 0BSD
 */

#include <dynarmic/A32/a32.h>
//#include "frontend/A32/location_descriptor.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include "utils.hpp"

using namespace Dynarmic;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

constexpr std::uint32_t RETURN_TO =   0x00000100;
constexpr std::uint32_t MEMORY_SIZE = 5 * 1024 * 1024;

class BenchEnvironment final : public A32::UserCallbacks {
public:
    u64 ticks_left = 0;
    std::array<u8, MEMORY_SIZE> memory{};
    bool finished = false;

    u8 MemoryRead8(u32 vaddr) override {
        if (vaddr >= memory.size()) {
            return 0;
        }
        return memory[vaddr];
    }

    u16 MemoryRead16(u32 vaddr) override {
        return u16(MemoryRead8(vaddr)) | u16(MemoryRead8(vaddr + 1)) << 8;
    }

    u32 MemoryRead32(u32 vaddr) override {
        return u32(MemoryRead16(vaddr)) | u32(MemoryRead16(vaddr + 2)) << 16;
    }

    u64 MemoryRead64(u32 vaddr) override {
        return u64(MemoryRead32(vaddr)) | u64(MemoryRead32(vaddr + 4)) << 32;
    }

    void MemoryWrite8(u32 vaddr, u8 value) override {
        if (vaddr >= memory.size()) {
            return;
        }
        memory[vaddr] = value;
    }

    void MemoryWrite16(u32 vaddr, u16 value) override {
        MemoryWrite8(vaddr, u8(value));
        MemoryWrite8(vaddr + 1, u8(value >> 8));
    }

    void MemoryWrite32(u32 vaddr, u32 value) override {
        MemoryWrite16(vaddr, u16(value));
        MemoryWrite16(vaddr + 2, u16(value >> 16));
    }

    void MemoryWrite64(u32 vaddr, u64 value) override {
        MemoryWrite32(vaddr, u32(value));
        MemoryWrite32(vaddr + 4, u32(value >> 32));
    }

    void InterpreterFallback(u32 , size_t ) override {
        // This is never called in practice.
        std::terminate();
    }

    void CallSVC(u32 ) override {
        // Do something.
    }

    void ExceptionRaised(u32 , Dynarmic::A32::Exception ) override {
        // Do something.
    }

    std::uint32_t MemoryReadCode(u32 vaddr) override { 
        if (vaddr==RETURN_TO) {
            finished = true;
        }
        return MemoryRead32(vaddr); 
    }

    void AddTicks(u64 ticks) override {
        if (ticks > ticks_left) {
            ticks_left = 0;
            return;
        }
        ticks_left -= ticks;
    }

    u64 GetTicksRemaining() override {
        return ticks_left;
    }
};



int main () {
    ElfReader elf("tester_arm");
    std::uint32_t ENTRY_POINT = elf.getSymbolAddress("_Z10findPrimesj");

    BenchEnvironment env;
    A32::UserConfig user_config;
    user_config.callbacks = &env;
    user_config.unsafe_optimizations = true;
    user_config.fastmem_pointer = (void*)env.memory.data();
    A32::Jit jit{user_config};

    elf.load( [&env](auto data, auto size, auto vaddr) {
        memcpy((char*)env.memory.data() + vaddr, data, size);
      });

    jit.Regs()[13] = MEMORY_SIZE - 128;  //SP
    jit.Regs()[15] = ENTRY_POINT;  //PC
    jit.Regs()[14] = RETURN_TO;  //LR
    jit.Regs()[0] = 100000;  //R0

    while (!env.finished) {
        env.ticks_left = 1024;
        jit.Run();
    }

    std::cout << "CPU state : R0=" << jit.Regs()[0] << std::endl;

    return 0;
}

