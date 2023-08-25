#pragma once

#include <code/code.h>

#include <cstddef>
#include <cstring>

#define malloc(x) _new(x)
//#define free(x) _delete(x)

void _delete(void* ptr) {
	code::Func<void, 0x0382ABB4, void*>()(ptr);
}

// Short Array Allocation
template<typename T>
T* _new(uint32_t size) {
    return code::Func<T*, 0x0382AACC, uint32_t>()(sizeof(T) * size);
}

inline void* operator new(size_t size) {
    return _new<char>(size);
}

inline void* operator new[](size_t size) {
    return _new<char>(size);
}