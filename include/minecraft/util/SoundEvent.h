#pragma once

namespace mc {
    class SoundEvent {
    public:
        static SoundEvent* block_chest_open      asm("0x104C4028");
        static SoundEvent* note_bd               asm("0x104C43A0");
        static SoundEvent* note_harp             asm("0x104C43A4");
        static SoundEvent* note_sand             asm("0x104C43A8");
        static SoundEvent* note_pling            asm("0x104C43AC");
        static SoundEvent* random_successful_hit asm("0x104C3FAC");
    };
}