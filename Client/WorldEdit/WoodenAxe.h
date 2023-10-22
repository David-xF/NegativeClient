#ifndef WOODEN_AXE_TOOL_H
#define WOODEN_AXE_TOOL_H

#include <minecraft/mc.h>

#include "Tool.h"

#define FOR_SPHERE(name) for (int name = -radius; name <= radius; name++)

class WorldEdit_WoodenAxe : public WorldEdit_Tool {
public:
    WorldEdit_WoodenAxe(mc::BlockPos* _pos1, mc::BlockPos* _pos2) : WorldEdit_Tool(itemId) {
        pos1 = _pos1;
        pos2 = _pos2;

        setPosMessage = L"[WorldEdit] Set Position %d to %d %d %d";
    }

    void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (hitPos.y != -1) {
            *pos1 = hitPos;
            mc_printf(setPosMessage, 1, hitPos.x, hitPos.y, hitPos.z);
        }
    }

    void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (hitPos.y != -1) {
            *pos2 = hitPos;
            mc_printf(setPosMessage, 2, hitPos.x, hitPos.y, hitPos.z);
        }
    }

    mc::Vec3 blockPosToVec3(mc::BlockPos pos) {
        return mc::Vec3(mc::toFloat(pos.x), mc::toFloat(pos.y), mc::toFloat(pos.z));
    }

    void draw3D(mc::BlockPos hitPos, bool wasForced = false) override {
        if (pos1->y == -1 || pos2->y == -1) return;
        if (!wasForced) return;

        mc::AABB box = mc::AABB(
            fmin(mc::toFloat(pos1->x), mc::toFloat(pos2->x)) - visualOffset,
            fmin(mc::toFloat(pos1->y), mc::toFloat(pos2->y)) - visualOffset,
            fmin(mc::toFloat(pos1->z), mc::toFloat(pos2->z)) - visualOffset,
            fmax(mc::toFloat(pos1->x), mc::toFloat(pos2->x)) + visualOffset + 1.0,
            fmax(mc::toFloat(pos1->y), mc::toFloat(pos2->y)) + visualOffset + 1.0,
            fmax(mc::toFloat(pos1->z), mc::toFloat(pos2->z)) + visualOffset + 1.0
        );

        drawLineBox(box);
    }   

    static constexpr int itemId = 271;

private:
    mc::BlockPos* pos1;
    mc::BlockPos* pos2;

    const wchar_t* setPosMessage;
};

#endif