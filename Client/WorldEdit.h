#ifndef WORLDEDIT_H
#define WORLDEDIT_H

#include "Module.h"

#include "WorldEdit/Tool.h"
#include "WorldEdit/StoneShovel.h"

#include <minecraft/mc.h>

struct WorldEdit* staticWorldEdit;

class WorldEdit {
public:
    struct BlockData {
        int id;
        int dmg;
        mc::Direction* dir;
    };

    WorldEdit() {
        _module = new Module(L"WorldEdit", Module::Type::MODULE);
        staticWorldEdit = this;

        _module->addModuleToSettings((new Module(L"Visualization", Module::Type::MODULE))->toggleState());

        selectedPos[0] = mc::BlockPos(0, -1, 0);
        selectedPos[1] = mc::BlockPos(0, -1, 0);
        selectedPos[2] = mc::BlockPos(0, -1, 0);
        blockData = xf::Vector<BlockData>();
        tools = xf::Vector<WorldEdit_Tool*>();
        Setting_sphereRadius = 3;

        tools.push_back(new WorldEdit_StoneShovel());
    }

    static void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand) {
        mc::BlockPos pos = staticWorldEdit->getPos();
        
        for (WorldEdit_Tool* tool : staticWorldEdit->getTools()) {
            if (mainHand->item->getId() == tool->getId()) {
                tool->onZL(mainHand, offHand, pos);
                break;
            }
        }
    }

    static void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand) {
        mc::BlockPos pos = staticWorldEdit->getPos();
        
        for (WorldEdit_Tool* tool : staticWorldEdit->getTools()) {
            if (mainHand->item->getId() == tool->getId()) {
                tool->onZR(mainHand, offHand, pos);
                break;
            }
        }
    }

    static void onFrame() {
        mc::BlockPos hit = mc::BlockPos(0, -1, 0);
        staticWorldEdit->getPos() = hit;
        if (!staticWorldEdit->getModule()->getState()) return;
        mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;

        mc::Vec3 lookAt = getLookAtVec(lPlayer->pitch, lPlayer->yaw);
        for (int i = 0; i < 600; i++) {
            mc::Vec3 temp = (lookAt * (mc::toFloat(i) * 0.1)) + lPlayer->position + mc::Vec3(0.0, lPlayer->eyeHeight, 0.0);
            int id = lPlayer->lvl->getBlockId(mc::toInt(temp.x), mc::toInt(temp.y), mc::toInt(temp.z));
            if (id != 0) {
                hit = mc::BlockPos(mc::toInt(temp.x), mc::toInt(temp.y), mc::toInt(temp.z));
                break;
            }
        }

        staticWorldEdit->getPos() = hit;

        mc_boost::shared_ptr<mc::ItemInstance> mainHand = lPlayer->inventory->getSelected();
        mc_boost::shared_ptr<mc::ItemInstance> offHand;
        lPlayer->inventory->getItem(40, offHand);
        START_TIMED_BUTTONCHECK(buttons & VPAD_BUTTON_ZR, onZR(mainHand.get(), offHand.get()), buttons, 200, 20)
        ADD_TIMED_BUTTONCHECK(buttons & VPAD_BUTTON_ZL, onZL(mainHand.get(), offHand.get()))
        END_TIMED_BUTTONCHECK();
    }

    static void render3D() {
        if (!staticWorldEdit->visualize()) return;
        
        mc::BlockPos pos = staticWorldEdit->getPos();
        if (pos.y != -1) {
            double offset = 0.05;
            xf::GUI::DrawHelper::DisplayBox3D(mc::toFloat(pos.x) - offset, mc::toFloat(pos.y) - offset, mc::toFloat(pos.z) - offset, 1.0 + (offset * 2.0), 1.0 + (offset * 2.0), 1.0 + (offset * 2.0), 0xBBBBBB, 0x66, true);
            
            for (WorldEdit_Tool* tool : staticWorldEdit->getTools()) {
                if (tool->getId() == mc::Minecraft::getInstance()->thePlayer->inventory->getSelected()->item->getId()) {
                    tool->draw3D(pos);
                    break;
                }
            }
        }
    }

    bool visualize() {
        return getModule()->getPageVector()[0]->getState();
    }

    xf::Vector<WorldEdit_Tool*>& getTools() {
        return tools;
    }

    mc::BlockPos& getPos(int i = 0) {
        return selectedPos[i];
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
    mc::BlockPos selectedPos[3];
    xf::Vector<BlockData> blockData;
    xf::Vector<WorldEdit_Tool*> tools;
    int Setting_sphereRadius;
};

#endif