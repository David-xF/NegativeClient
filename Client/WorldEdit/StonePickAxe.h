#ifndef STONE_PICKAXE_TOOL_H
#define STONE_PICKAXE_TOOL_H

#include <minecraft/mc.h>

#include <xf/String.h>

#include "Tool.h"

#define FOR_SPHERE_PICK_AXE(name) for (int name = min_##name; name < max_##name; name++)

class WorldEdit_StonePickAxe : public WorldEdit_Tool {
public:
    WorldEdit_StonePickAxe(mc::BlockPos* _pos1, mc::BlockPos* _pos2) : WorldEdit_Tool(itemId) {
        pos1 = _pos1;
        pos2 = _pos2;
    }

    void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (pos1->y == -1 || pos2->y == -1) return;

        int min_x = we_min(pos1->x, pos2->x);
        int min_y = we_min(pos1->y, pos2->y);
        int min_z = we_min(pos1->z, pos2->z);
        int max_x = we_max(pos1->x, pos2->x) + 1;
        int max_y = we_max(pos1->y, pos2->y) + 1;
        int max_z = we_max(pos1->z, pos2->z) + 1;   

        mc::Vec3 lastTP = mc::Vec3(-50.0f, -50.0f, -50.0f);

        FOR_SPHERE_PICK_AXE(x) {
            FOR_SPHERE_PICK_AXE(y) {
                FOR_SPHERE_PICK_AXE(z) {
                    mc::Vec3 vec(mc::toFloat(x), mc::toFloat(y), mc::toFloat(z));
                    if (vec.distance(lastTP) >= 5.0f) {
                        vec = vec + mc::Vec3(2.0, 2.0, 2.0);
                        teleportPacket(vec);
                        lastTP = vec;
                    }

                    int id = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockId(x, y, z);
                    int aux = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockData(x, y, z);
                    if (offHand->item->getId() == 0 || offHand->getCount() == 0) {
                        if (id != 0) {
                            breakBlock({x, y, z}, mc::ServerboundPlayerActionPacket::Action::START_BREAKING);
                        }
                    } else {
                        if (id != offHand->item->getId() || aux != offHand->aux) {
                            if (id != 0) {
                                breakBlock({x, y, z}, mc::ServerboundPlayerActionPacket::Action::START_BREAKING);
                            } 

                            placeBlock({x, y, z}, mc::Direction::north, mc::InteractionHand::EInteractionHand::OFF_HAND);
                        }
                    }
                }
            }
        }
    }

    void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        
    }

    void draw3D(mc::BlockPos hitPos, bool wasForced = false) override {
        
    }   

    static constexpr int itemId = 274;

private:
    mc::BlockPos* pos1;
    mc::BlockPos* pos2;
};

#endif