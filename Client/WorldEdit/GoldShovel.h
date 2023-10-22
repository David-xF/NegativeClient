#ifndef GOLD_SHOVEL_TOOL_H
#define GOLD_SHOVEL_TOOL_H

#include <minecraft/mc.h>

#include <xf/String.h>

#include "Tool.h"

#define FOR_SPHERE(name) for (int name = -radius; name <= radius; name++)

class WorldEdit_GoldShovel : public WorldEdit_Tool {
public:
    WorldEdit_GoldShovel() : WorldEdit_Tool(itemId) {
        radius = 7;
    }

    void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (hitPos.y == -1) return;

        FOR_SPHERE(x) {
            FOR_SPHERE(y) {
                FOR_SPHERE(z) {
                    mc::BlockPos position = mc::BlockPos(hitPos.x + x, hitPos.y + y, hitPos.z + z);
                    mc::Level* level = mc::Minecraft::getInstance()->thePlayer->lvl;
                    if (level->getBlockId(position.x, position.y, position.z) != 0) {
                        int t = 1;
                        while ((position.y - t + 1) > 0) {
                            if (level->getBlockId(position.x, position.y - t, position.z)) break;
                            t++;
                        }
                        
                        if (t > 1) {
                            int32_t id = level->getBlockId(position.x, position.y, position.z);
                            int32_t aux = level->getBlockData(position.x, position.y, position.z);
                            teleportPacket({mc::toFloat(position.x), mc::toFloat(position.y), mc::toFloat(position.x)});
                            breakBlock(position, mc::ServerboundPlayerActionPacket::Action::START_BREAKING);
                            mc_boost::shared_ptr<mc::Packet> packet1(new mc::ServerboundSetCreativeModeSlotPacket(45, new mc::ItemInstance(mc::Item::byId(id), 1, aux)));
                            mc::Minecraft::getInstance()->getConnection(0)->send(packet1);
                            teleportPacket({mc::toFloat(position.x + 2), mc::toFloat(position.y - t + 3), mc::toFloat(position.x + 2)});
                            placeBlock({position.x, position.y - t + 1, position.z}, mc::Direction::north, mc::InteractionHand::EInteractionHand::OFF_HAND, new mc::ItemInstance(mc::Item::byId(id), 1, aux));
                            mc_boost::shared_ptr<mc::Packet> packet2(new mc::ServerboundSetCreativeModeSlotPacket(45, offHand));
                            mc::Minecraft::getInstance()->getConnection(0)->send(packet2);
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

    static constexpr int itemId = 284;

private:
    int radius;
};

#endif