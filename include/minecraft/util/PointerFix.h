#pragma once

#include <code/code.h>

extern "C" {
    uint32_t text_start;
    uint32_t text_end;
};

namespace mc {
    class PointerFix {
    public: 
        template<typename T>
        static T* Fix(T* addr) {
        	if (((uint32_t) addr >= (uint32_t) &text_start) && (uint32_t) &text_end >= (uint32_t) addr)
            	return (T*) (code::Mem(0x104D4DD8).as<uint32_t>() + ((uint32_t) addr - (uint32_t) &text_start));  
        	return (T*) addr;
        }
    };
}