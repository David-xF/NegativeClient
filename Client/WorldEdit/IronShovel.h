#ifndef IRON_SHOVEL_TOOL_H
#define IRON_SHOVEL_TOOL_H

#include <minecraft/mc.h>

#include <xf/String.h>

#include "Tool.h"

#define FOR_SPHERE(name) for (int name = -radius; name <= radius; name++)

class WorldEdit_IronShovel : public WorldEdit_Tool {
public:
    enum SmoothMode {
        PLACE, BREAK
    };

    WorldEdit_IronShovel(Slider<int>* _slider) : WorldEdit_Tool(itemId) {
        radius = 7;
        mode = SmoothMode::PLACE;
        slider = _slider;
    }

    bool isValidPlace(mc::BlockPos& position) {
        mc::BlockPos positions[6] = {
            {  1,  0,   0},
            { -1,  0,   0},
            { 0,   1,   0},
            { 0,  -1,   0},
            { 0,   0,   1},
            { 0,   0,  -1}
        };

        int hits = 0;
        int minHits = slider->getCurrent();
        mc::Level* level = mc::Minecraft::getInstance()->thePlayer->lvl;
        if (mode == SmoothMode::PLACE) {
            if (level->getBlockId(position.x, position.y, position.z) == 0) {
                for (mc::BlockPos& _pos : positions) {
                    mc::BlockPos tempPos = position + _pos;
                    if (level->getBlockId(tempPos.x, tempPos.y, tempPos.z) != 0) hits++;
                }

                if (minHits < hits) return true;
            }
        } else {
            if (level->getBlockId(position.x, position.y, position.z) != 0) {
                for (mc::BlockPos& _pos : positions) {
                    mc::BlockPos tempPos = position + _pos;
                    if (level->getBlockId(tempPos.x, tempPos.y, tempPos.z) == 0) hits++;
                }

                if (minHits < hits) return true;
            }
        }

        return false;
    }

    void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (hitPos.y == -1) return;

        xf::Vector<mc::BlockPos> interactionBlocks;

        FOR_SPHERE(x) {
            FOR_SPHERE(y) {
                FOR_SPHERE(z) {
                    mc::BlockPos position = mc::BlockPos(hitPos.x + x, hitPos.y + y, hitPos.z + z);
                    if (isValidPlace(position)) interactionBlocks.push_back(position);
                }
            }
        }

        for (mc::BlockPos pos : interactionBlocks) {
            mc::Vec3 vec = mc::Vec3(mc::toFloat(pos.x), mc::toFloat(pos.y + 2), mc::toFloat(pos.z));
            teleportPacket(vec);
            if (mode == SmoothMode::PLACE) {
                placeBlock(pos, mc::Direction::up, mc::InteractionHand::EInteractionHand::OFF_HAND, offHand);
            } else {
                breakBlock(pos, mc::ServerboundPlayerActionPacket::Action::START_BREAKING);
            }
        }

        interactionBlocks.clear();
    }

    void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        const char* before;
        const char* now;
        if (mode == SmoothMode::PLACE) {
            mode = SmoothMode::BREAK;
            before = "Place";
            now = "Break";
        } else {
            mode = SmoothMode::PLACE;
            before = "Break";
            now = "Place";
        }
        
        mc_printf(L"[WorldEdit] Mode: %s >> %s", before, now);
        mc::ConsoleUIController::getInstance()->PlayUISFX(mc::SoundEvent::block_note_xylophone);
    }

    void draw3D(mc::BlockPos hitPos, bool wasForced = false) override {
        if (hitPos.y == -1) return;

        FOR_SPHERE(x) {
            FOR_SPHERE(y) {
                FOR_SPHERE(z) {
                    mc::BlockPos position = mc::BlockPos(hitPos.x + x, hitPos.y + y, hitPos.z + z);
                    if (isValidPlace(position)) {
                        drawLineBox({position.x, position.y, position.z, position.x + 1, position.y + 1, position.z + 1});
                    }
                }
            }
        }
    }   

    static constexpr int itemId = 256;

private:
    int radius;
    SmoothMode mode;
    Slider<int>* slider;
};

#endif