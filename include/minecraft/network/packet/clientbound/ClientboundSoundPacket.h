#pragma once

#include <code/code.h>

#include "../Packet.h"
#include "../../../util/SoundEvent.h"

namespace mc {
    class ClientboundSoundPacket : public Packet {
    public:
        // ClientboundSoundPacket::ClientboundSoundPacket(SoundEvent const *, SoundSource::ESoundSource, double, double, double, float, float)
        ClientboundSoundPacket(SoundEvent* sound, double x, double y, double z, float unk_0x0, float unk_0x1) {
            code::Func<void, 0x02229070, ClientboundSoundPacket*, SoundEvent*, int, double, double, double, float, float>()(this, sound, 0, x, y, z, unk_0x0, unk_0x1);
        }

        ClientboundSoundPacket(SoundEvent* sound, float vol, float pitch) {
            code::Func<void, 0x02229214, ClientboundSoundPacket*, SoundEvent*, float, float>()(this, sound, vol, pitch);
        }

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
        uint32_t field_0x38;
        uint32_t field_0x3C;
    };
}