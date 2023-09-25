#pragma once

#include "mem.h"
#include <xf/Vector.h>

void writeMem(uint32_t addr, uint32_t value) {
    code::Mem(addr).as<uint32_t>() = value;
}

uint32_t branchTo(uint32_t origin, void* to) {
    uint32_t _addr = (uint32_t) to;
    uint32_t temp = (_addr - origin);
    if (0x48000000 > (0x48000000 + temp)) {
        return 0x4C000000 + temp;
    } else {
        return 0x48000000 + temp;
    }
}

xf::Vector<void**> real_instructions;
const int MaxTest = 0x100;
int __test_index = 0;
uint32_t instructionBuffer[MaxTest];
void* setupRealInstructions(uint32_t* buffer, uint32_t startAddr) {
    uint32_t endFor = (uint32_t) &instructionBuffer[__test_index];
    for (int i = 0; i < buffer[0] + 1; i++) {
        uint32_t writeAddr = endFor + (i * 4);
        if (buffer[1 + i] == 0xFFFFFFFF) writeMem(writeAddr, branchTo(writeAddr, (void*) (startAddr + 4)));
        else                             writeMem(writeAddr, buffer[1 + i]);
    }

    __test_index += (buffer[0] + 1);

    return (void*) (endFor);
}

void* safeInstructions(uint32_t startFunction) {
    xf::Vector<uint32_t> instructions;
    instructions.push_back(code::Mem(startFunction).as<uint32_t>());
    instructions.push_back(0xFFFFFFFF);
   
    uint32_t* ret = new uint32_t[instructions.getSize() + 1];
    int i = 1;
    for (uint32_t t : instructions) {
        ret[i++] = t;
    }

    ret[0] = instructions.getSize();
    return setupRealInstructions(ret, startFunction);
}

#define DECL_HOOK(func, ...) \
    void hook_##func(__VA_ARGS__) 

void _TEST(uint32_t addr, uint32_t funcPtr, int offset) {
    uint32_t hookPtr = (uint32_t) &instructionBuffer[__test_index];
    uint32_t fFuncPtr = (uint32_t) funcPtr;
    writeMem(addr, branchTo(addr, (void*) hookPtr));
    writeMem(hookPtr + 0x0,  0x4E800421);                                              // bctrl;
    writeMem(hookPtr + 0x4,  0x3D800000 | (fFuncPtr >> 16));                           // lis r12, 0x;
    writeMem(hookPtr + 0x8,  0x618C0000 | (fFuncPtr & 0xFFFF));                        // ori r12, r12, 0x;
    writeMem(hookPtr + 0xC,  0x7D8903A6);                                              // mtctr r12;
    writeMem(hookPtr + 0x10, 0x4E800421);                                              // bctrl;
    writeMem(hookPtr + 0x14, branchTo(hookPtr + 0x14, (void*) (addr + 0x4 + offset))); // b addr+0x4;
    __test_index += 8;
}

#define HOOK(addr, func, offset) \
    _TEST(addr, (uint32_t) hook_##func, offset);
    
#define REPLACE(sAddr, func) REPLACE_EX(sAddr, real_##func, my_##func, func, real_instructions)
#define REPLACE_EX(sAddr, original_func, replace_func, func, ins_list) \
    auto lambda_##func = [&]() {                                       \
        void* _instructions = safeInstructions(sAddr);                 \
        writeMem(sAddr, branchTo(sAddr, (void*) &(replace_func)));     \
        void** __data = (void**) new uint32_t[2];                      \
        __data[0] = (void*) &(original_func);                          \
        __data[1] = _instructions;                                     \
        ins_list.push_back(__data);                                    \
    };                                                                 \
    lambda_##func();                                                         

/*
 * Original code by Chadderz
 *
 * #define declareFunctionHook(returnType, functionName, ...) \
 *      returnType (* real_ ## functionName)(__VA_ARGS__) __attribute__((section(".data"))); \
 *      returnType my_ ## functionName(__VA_ARGS__)
 *
 */

// Found in WUPS then TCPGecko (Expanded by david.xf)
#define DECL_FUNCTION(res, name, ...)                      \
    res real_##name(__VA_ARGS__) {                         \
        asm volatile("_" #name "Start:");                  \
        asm volatile("stwu 1, -0x100(1)");                 \
        asm volatile("mflr 0");                            \
        asm volatile("stw 0, 0x104(1)");                   \
        asm volatile("stw 30, 0x20(1)");                   \
        asm volatile("stw 29, 0x24(1)");                   \
        asm volatile("stw 28, 0x28(1)");                   \
        asm volatile("stw 27, 0x2C(1)");                   \
        asm volatile("stw 26, 0x30(1)");                   \
        asm volatile("stw 25, 0x34(1)");                   \
        asm volatile("stw 24, 0x38(1)");                   \
        asm volatile("stw 23, 0x3C(1)");                   \
        asm volatile("stw 22, 0x40(1)");                   \
        asm volatile("stw 21, 0x44(1)");                   \
        asm volatile("stw 20, 0x48(1)");                   \
        asm volatile("lis 30, 0x104D");                    \
        asm volatile("ori 30, 30, 0x4DDC");                \
        asm volatile("lwz 30, 0(30)");                     \
        asm volatile("lwz 30, 0(30)");                     \
        asm volatile("lwz 29, 0(30)");                     \
        asm volatile("lwz 28, 4(30)");                     \
        asm volatile("mr 27, 28");                         \
        asm volatile("li 26, 0");                          \
        asm volatile("li 25, 4");                          \
        asm volatile("_" #name ":");                       \
        asm volatile("mullw 24, 26, 25");                  \
        asm volatile("add 23, 29, 24");                    \
        asm volatile("lwz 23, 0(23)");                     \
        asm volatile("lis 22, " "_" #name "Start@ha");     \
        asm volatile("addi 22, 22, " "_" #name "Start@l"); \
        asm volatile("lwz 30, 0(23)");                     \
        asm volatile("cmpw 22, 30");                       \
        asm volatile("beq _" #name "Found");               \
        asm volatile("addi 26, 26, 1");                    \
        asm volatile("b _" #name);                         \
        asm volatile("_" #name "Found:");                  \
        asm volatile("lwz 21, 4(23)");                     \
        asm volatile("mtctr 21");                          \
        asm volatile("bctrl");                             \
        asm volatile("lwz 30, 0x20(1)");                   \
        asm volatile("lwz 29, 0x24(1)");                   \
        asm volatile("lwz 28, 0x28(1)");                   \
        asm volatile("lwz 27, 0x2C(1)");                   \
        asm volatile("lwz 26, 0x30(1)");                   \
        asm volatile("lwz 25, 0x34(1)");                   \
        asm volatile("lwz 24, 0x38(1)");                   \
        asm volatile("lwz 23, 0x3C(1)");                   \
        asm volatile("lwz 22, 0x40(1)");                   \
        asm volatile("lwz 21, 0x44(1)");                   \
        asm volatile("lwz 20, 0x48(1)");                   \
        asm volatile("addi 1, 1, 0x100");                  \
        asm volatile("lwz 0, 4(1)");                       \
        asm volatile("mtlr 0");                            \
        asm volatile("blr");                               \
        return res();                                      \
    }                                                      \
    res my_##name(__VA_ARGS__)

void InitWups() {
    uint32_t* data = new uint32_t[0x10];
    code::Mem(0x104D4DDC).as<uint32_t>() = (uint32_t) data;
    data[0] = (uint32_t) &real_instructions;
}