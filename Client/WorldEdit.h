#ifndef WORLDEDIT_H
#define WORLDEDIT_H

#include "Module.h"

#include <minecraft/mc.h>

struct WorldEdit* staticWorldEdit;

class WorldEdit {
public:
    WorldEdit() {
        _module = new Module(L"WorldEdit", Module::Type::MODULE);
        staticWorldEdit = this;

        _module->addModuleToSettings((new Module(L"Visualization", Module::Type::MODULE))->toggleState());

        selectedPos = mc::BlockPos(0, 0, 0);
    }

    static mc::Vec3 getLookAtVec(float pitch, float yaw) {
        mc::Vec3 ret = {
            fabs(sinf(degToRad(pitch - 90.0f))) * sinf(degToRad(yaw   - 180.0f)),
                                                  sinf(degToRad(pitch - 180.0f)),
            fabs(sinf(degToRad(pitch - 90.0f))) * cosf(degToRad(yaw))
        };

        return ret;
    }

    static void onFrame() {
        if (!staticWorldEdit->getModule()->getState()) return;
        mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;

        mc::Vec3 lookAt = getLookAtVec(lPlayer->pitch, lPlayer->yaw);
        mc::BlockPos hit = mc::BlockPos(0, -1, 0);
        for (int i = 0; i < 600; i++) {
            mc::Vec3 temp = (lookAt * (mc::toFloat(i) * 0.1)) + lPlayer->position + mc::Vec3(0.0, lPlayer->eyeHeight, 0.0);
            int id = lPlayer->lvl->getBlockId(mc::toInt(temp.x), mc::toInt(temp.y), mc::toInt(temp.z));
            if (id != 0) {
                hit = mc::BlockPos(mc::toInt(temp.x), mc::toInt(temp.y), mc::toInt(temp.z));
                break;
            }
        }

        staticWorldEdit->getPos() = hit;
    }

    static void renderCircle(mc::BlockPos pos, float radius, int ySeg, int xSeg) {
        mc::Vec3 startPos = mc::Vec3(pos.x, pos.y, pos.z);

        float xR = 360.0f / mc::toFloat(xSeg);
        float yR = 180.0f / mc::toFloat(ySeg);
        for (int j = 0; j < ySeg; j++) {
            for (int i = 0; i < xSeg; i++) {
                mc::Vec3 lookAt1 = (getLookAtVec((yR * mc::toFloat(i)) - 90.0f,     xR * mc::toFloat(i))     * radius) + startPos + mc::Vec3(0.0, 0.5, 0.0);
                mc::Vec3 lookAt2 = (getLookAtVec((yR * mc::toFloat(i)) - 90.0f,     xR * mc::toFloat(i + 1)) * radius) + startPos + mc::Vec3(0.0, 0.5, 0.0);
                mc::Vec3 lookAt3 = (getLookAtVec((yR * mc::toFloat(i + 1)) - 90.0f, xR * mc::toFloat(i))     * radius) + startPos + mc::Vec3(0.0, 0.5, 0.0);
                mc::Vec3 lookAt4 = (getLookAtVec((yR * mc::toFloat(i + 1)) - 90.0f, xR * mc::toFloat(i + 1)) * radius) + startPos + mc::Vec3(0.0, 0.5, 0.0);

                drawLine(lookAt1, lookAt2);
                drawLine(lookAt1, lookAt3);
                drawLine(lookAt3, lookAt4);
                drawLine(lookAt2, lookAt4);
            }
        }
    }

    static void render3D() {
        if (!staticWorldEdit->visualize()) return;
        
        mc::BlockPos pos = staticWorldEdit->getPos();
        if (pos.y != -1) {
            double offset = 0.05;
            xf::GUI::DrawHelper::DisplayBox3D(mc::toFloat(pos.x) - offset, mc::toFloat(pos.y) - offset, mc::toFloat(pos.z) - offset, 1.0 + (offset * 2.0), 1.0 + (offset * 2.0), 1.0 + (offset * 2.0), 0xBBBBBB, 0x66, true);
            renderCircle(pos, 5.0f, 20, 20);
        }
    }

    static void drawLine(mc::Vec3 pos1, mc::Vec3 pos2) {
        mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
        mc::GlStateManager::disableTexture();
        mc::GlStateManager::lineWidth(5);
        mc::GlStateManager::pushMatrix();
        builder->begin(MC_GL_LINES);
        builder->vertex(pos1.x, pos1.y, pos1.z, true);
        builder->vertex(pos2.x, pos2.y, pos2.z, true);
        builder->end();
        mc::GlStateManager::popMatrix();
    }

    bool visualize() {
        return getModule()->getPageVector()[0]->getState();
    }

    mc::BlockPos& getPos() {
        return selectedPos;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
    mc::BlockPos selectedPos;
};

#endif