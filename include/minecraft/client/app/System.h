#pragma once

#include <code/code.h>

namespace mc {
    class System {
    public:
        static uint64_t processTimeInMilliSecsu64() {
            return code::Func<uint64_t, 0x02994408>()();
        }
    };
}