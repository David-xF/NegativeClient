#pragma once

#include <code/code.h>

#include "../client/resource/wstring.h"

namespace mc {
    class Gui {
    public:
        void addMessage(const mstd::wstring& wstr, int unk_0x0, bool unk_0x1, bool unk_0x2, bool unk_0x3, bool unk_0x4) {
            code::Func<void, 0x0313873C, Gui*, const mstd::wstring&, uint32_t, bool, bool, bool, bool>()(this, wstr, unk_0x0, unk_0x1, unk_0x2, unk_0x3, unk_0x4);
        }
    };
};