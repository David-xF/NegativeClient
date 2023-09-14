#pragma once

#include <code/code.h>
#include <inttypes.h>

namespace mstd {
    class wstring {
    public:
        wstring(const wchar_t* str) {
            code::Func<void, 0x020B08D4, wstring*, const wchar_t*>()(this, str);
        }

        wstring() = default;
        
        const wchar_t* c_str() const {
            return (length > 7) ? pointer : str;
        }

        uint32_t unk_0x0;    // 0
        wchar_t str[8];     // 4
        wchar_t* pointer;   // 14
        uint32_t length; // 18
        uint32_t length2;    // 1C
    };
}