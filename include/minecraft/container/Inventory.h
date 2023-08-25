#pragma once

#include <code/code.h>

#include "Container.h"

#include "../client/resource/shared_ptr.h"
#include "../client/resource/NonNullList.h"

namespace mc {
    class Inventory : public Container {
    public:
        void dropAll() {
            code::Func<void, 0x024a1878, Inventory*>()(this);
        }

        void clearContent() {
            code::Func<void, 0x024A2C94, Inventory*>()(this);
        }

        void setItem(uint32_t index, mc_boost::shared_ptr<struct ItemInstance> const & item) {
            code::Func<void, 0x0249efac, Inventory*, uint32_t, mc_boost::shared_ptr<struct ItemInstance> const &>()(this, index, item);
        }

        void setChanged() {
            code::Func<void, 0x024a20e8, Inventory*>()(this);
        }

        mc_boost::shared_ptr<struct ItemInstance> const & getSelected() {
            return code::Func<mc_boost::shared_ptr<struct ItemInstance> const &, 0x02497F7C, Inventory*>()(this);
        }

        void add(int slot, mc_boost::shared_ptr<struct ItemInstance> const & item) {
            code::Func<void, 0x0249C0D4, Inventory*, int, mc_boost::shared_ptr<struct ItemInstance> const &>()(this, slot, item);
        }

        void getItem(uint32_t slot, mc_boost::shared_ptr<struct ItemInstance> const & item) {
            code::Func<void, 0x024a0b9c, Inventory*, mc_boost::shared_ptr<struct ItemInstance> const &, uint32_t>()(this, item, slot);
        }

        uint32_t field_0xC;
        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
        uint32_t field_0x20;
        uint32_t field_0x24;
        uint32_t field_0x28;
        uint32_t field_0x2C;
        uint32_t field_0x30;
        uint32_t field_0x34;
        NonNullList<mc_boost::shared_ptr<struct ItemInstance>> list;
        uint32_t field_0x48;
        uint32_t field_0x4C;
        uint32_t field_0x50;
        uint32_t field_0x54;
        uint32_t field_0x58;
        uint32_t field_0x5C;
        uint32_t field_0x60;
        uint32_t field_0x64;
        uint32_t field_0x68;
        uint32_t idk;
        uint32_t field_0x70;
        uint32_t field_0x74;
        uint32_t field_0x78;
        uint32_t field_0x7C;
        uint32_t field_0x80;
        uint32_t field_0x84;
        uint32_t field_0x88;
    };
}