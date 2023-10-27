#ifndef STICK_TOOL_H
#define STICK_TOOL_H

#include <minecraft/mc.h>

#include "Tool.h"

class WorldEdit_Stick : public WorldEdit_Tool {
public:
    WorldEdit_Stick() : WorldEdit_Tool(itemId) {
    }

    void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {

    }

    void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (hitPos.y == -1) return;

        mc::Level* level = mc::Minecraft::getInstance()->thePlayer->lvl;
        int id = level->getBlockId(hitPos.x, hitPos.y, hitPos.z);
        int aux = level->getBlockData(hitPos.x, hitPos.y, hitPos.z);
        mc_printf(L"[WorldEdit] Block Data Position %d %d %d | ID: %d | Aux: %d", hitPos.x, hitPos.y, hitPos.z, id, aux);
    }

    void draw3D(mc::BlockPos hitPos, bool wasForced = false) override {
        if (hitPos.y == -1) return;
    }   

    static constexpr int itemId = 280;

private:
};

#endif