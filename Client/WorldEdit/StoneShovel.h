#ifndef STONE_SHOVEL_TOOL_H
#define STONE_SHOVEL_TOOL_H

#include <minecraft/mc.h>

#include "Tool.h"

#define FOR_SPHERE(name) for (int name = -radius; name <= radius; name++)

class WorldEdit_StoneShovel : public WorldEdit_Tool {
public:
    WorldEdit_StoneShovel() : WorldEdit_Tool(itemId) {
        radius = 3;
    }

    void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (hitPos.y == -1) return;

        mc::Vec3 lastTP = mc::Vec3(-50.0f, -50.0f, -50.0f);

        FOR_SPHERE(x) {
            FOR_SPHERE(y) {
                FOR_SPHERE(z) {
                    mc::BlockPos position = mc::BlockPos(hitPos.x + x, hitPos.y + y, hitPos.z + z);
                    mc::Vec3 vec = mc::Vec3(mc::toFloat(position.x), mc::toFloat(position.y), mc::toFloat(position.z));
                    if (mc::toFloat(radius) >= vec.distance({mc::toFloat(hitPos.x), mc::toFloat(hitPos.y), mc::toFloat(hitPos.z)})) {
                        if (vec.distance(lastTP) >= 5.0f) {
                            vec = vec + mc::Vec3(2.0, 2.0, 2.0);
                            teleportPacket(vec);
                            lastTP = vec;
                        }

                        int id = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockId(position.x, position.y, position.z);
                        if (offHand->item->getId() == 0 || offHand->getCount() == 0) {
                            if (id != 0) {
                                breakBlock(position, mc::ServerboundPlayerActionPacket::Action::START_BREAKING);
                            }
                        } else {
                            if (id != 0) {
                                breakBlock(position, mc::ServerboundPlayerActionPacket::Action::START_BREAKING);
                            } 

                            placeBlock(position, mc::Direction::north, mc::InteractionHand::EInteractionHand::OFF_HAND, offHand);
                        }
                    }
                }
            }
        }
    }

    void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        int before = radius++;
        if (radius > 8) radius = 1;
        
        mc_printf(L"[WorldEdit] Radius: %d >> %d", before, radius);
        mc::ConsoleUIController::getInstance()->PlayUISFX(mc::SoundEvent::block_note_xylophone);
    }

    void draw3D(mc::BlockPos hitPos, bool wasForced = false) override {
        int r = (radius * 2) + 4;
        renderSphere(hitPos, mc::toFloat(radius) + 0.5, r, r);
    }   

    static void renderSphere(mc::BlockPos pos, float radius, int ySeg, int xSeg) {
        mc::Vec3 startPos = mc::Vec3(pos.x, pos.y, pos.z);
        
        mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
        mc::GlStateManager::disableTexture();
        mc::GlStateManager::lineWidth(5);
        mc::GlStateManager::pushMatrix();
        builder->begin(MC_GL_LINES);

        float xR = 360.0f / mc::toFloat(xSeg);
        float yR = 180.0f / mc::toFloat(ySeg);
        for (int j = 0; j < ySeg; j++) {
            for (int i = 0; i < xSeg; i++) {
                mc::Vec3 lookAt1 = (getLookAtVec((yR * mc::toFloat(j)) - 90.0f,     xR * mc::toFloat(i))     * radius) + startPos + mc::Vec3(0.0, 0.5, 0.0);
                mc::Vec3 lookAt2 = (getLookAtVec((yR * mc::toFloat(j)) - 90.0f,     xR * mc::toFloat(i + 1)) * radius) + startPos + mc::Vec3(0.0, 0.5, 0.0);
                mc::Vec3 lookAt3 = (getLookAtVec((yR * mc::toFloat(j + 1)) - 90.0f, xR * mc::toFloat(i))     * radius) + startPos + mc::Vec3(0.0, 0.5, 0.0);
                mc::Vec3 lookAt4 = (getLookAtVec((yR * mc::toFloat(j + 1)) - 90.0f, xR * mc::toFloat(i + 1)) * radius) + startPos + mc::Vec3(0.0, 0.5, 0.0);

                drawLine(builder, lookAt1, lookAt2);
                drawLine(builder, lookAt1, lookAt3);
                drawLine(builder, lookAt3, lookAt4);
                drawLine(builder, lookAt2, lookAt4);
            }
        }

        builder->end();
        mc::GlStateManager::popMatrix();
    }

    static constexpr int itemId = 273;

private:
    int radius;
};

#endif