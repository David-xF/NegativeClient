#pragma once

#include <code/code.h>

#include "../client/resource/wstring.h"

namespace mc {
    class SoundEvent {
    public:
        static SoundEvent* random_successful_hit         asm("0x104C3FAC");
        static SoundEvent* random_bow                    asm("0x104C3FB0");
        static SoundEvent* mob_bat_idle                  asm("0x104C3FB4");
        static SoundEvent* mob_bat_death                 asm("0x104C3FB8");
        static SoundEvent* mob_bat_hurt                  asm("0x104C3FBC");
        static SoundEvent* mob_bat_takeoff               asm("0x104C3FC0");
        static SoundEvent* mob_blaze_breath              asm("0x104C3FC4");
        static SoundEvent* fire_fire                     asm("0x104C3FC8");
        static SoundEvent* mob_blaze_death               asm("0x104C3FCC");
        static SoundEvent* mob_blaze_hit                 asm("0x104C3FD0");
        static SoundEvent* mob_ghast_fireball            asm("0x104C3FD4");
        static SoundEvent* entity_bobber_throw           asm("0x104C3FD8");
        static SoundEvent* entity_bobber_retrieve        asm("0x104C3FDC");
        static SoundEvent* item_bottle_empty             asm("0x104C3FE0");
        static SoundEvent* item_bottle_fill              asm("0x104C3FE4");
        static SoundEvent* item_bottle_fill_dragonbreath asm("0x104C3FE8");
        static SoundEvent* block_brewing_stand_brew      asm("0x104C3FEC");
        static SoundEvent* item_bucket_empty             asm("0x104C3FF0");
        static SoundEvent* item_bucket_empty_lava        asm("0x104C3FF4");
        static SoundEvent* item_bucket_fill              asm("0x104C3FF8");
        static SoundEvent* item_bucket_fill_lava         asm("0x104C3FFC");
        static SoundEvent* item_bucket_fill_fish         asm("0x104C4000");
        static SoundEvent* block_chest_open              asm("0x104C4028");
        static SoundEvent* random_fizz                   asm("0x104C430C");
        static SoundEvent* ambient_weather_thunder       asm("0x104C4324");
        static SoundEvent* note_bd                       asm("0x104C439C");
        static SoundEvent* note_bassattack               asm("0x104C43A0");
        static SoundEvent* note_harp                     asm("0x104C43A4");
        static SoundEvent* note_hat                      asm("0x104C43A8");
        static SoundEvent* note_pling                    asm("0x104C43AC");
        static SoundEvent* random_click                  asm("0x104C4370");
        static SoundEvent* note_snare                    asm("0x104C43B0");
        static SoundEvent* random_breath                 asm("0x104C4478");
        static SoundEvent* random_break                  asm("0x104C453C");
        static SoundEvent* MG01_celebration              asm("0x104C47CC");
        static SoundEvent* MG01_death                    asm("0x104C47D4");
        static SoundEvent* MG01_gracezero                asm("0x104C47DC");
        static SoundEvent* MG01_lobby                    asm("0x104C47E0");
        static SoundEvent* MG01_lobbyaccent              asm("0x104C47E4");
        static SoundEvent* MG01_lobbyzero                asm("0x104C47E8");
        static SoundEvent* MG01_gamestart                asm("0x104C47EC");
        static SoundEvent* MG02_gamestart                asm("0x104C47F8");
        static SoundEvent* MG03_emeraldPointTick         asm("0x104C4864");
        static SoundEvent* block_note_xylophone          asm("0x104C488C");
        static SoundEvent* block_note_bell               asm("0x104C4890");
        static SoundEvent* block_note_chime              asm("0x104C4894");
        static SoundEvent* block_note_flute              asm("0x104C4898");
        static SoundEvent* block_note_guitar             asm("0x104C489C");
        static SoundEvent* item_trident_riptide2         asm("0x104C4954");

        void getName(const mstd::wstring& name) {
            code::Func<void, 0x028E11EC, SoundEvent*, const mstd::wstring&>()(this, name);
        }
    };
}