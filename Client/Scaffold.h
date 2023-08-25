#ifndef SCAFFOLD_H
#define SCAFFOLD_H

#include <code/code.h>
#include "Module.h"

#include <minecraft/mc.h>

void* staticScaffold;

class Scaffold {
public:
    Scaffold() {
        _module = new Module(L"Scaffold", Module::Type::MODULE);
        staticScaffold = this;

        blockPos = {0, -1, 0};
    }

    static void onTick() {
        Scaffold* scaffold = (Scaffold*) staticScaffold;
        if (!scaffold->getModule()->getState()) return;

        mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
        mc::ItemInstance* item;
        item = lPlayer->inventory->getSelected().ptr;
        int x = mc::toInt(lPlayer->position.x);        
        int y = mc::toInt(lPlayer->position.y) - 1;
        int z = mc::toInt(lPlayer->position.z);
        if (lPlayer->lvl->getBlockId(x, y, z) != 0) return;
        if (!lPlayer->isAllowedToUse(mc::Block::byId(item->item->getId()))) return;
        mc::BlockPos t[6] = {
            {x + 1, y + 0, z + 0},
            {x + 0, y + 1, z + 0},
            {x + 0, y + 0, z + 1},
            {x - 1, y - 0, z - 0},
            {x - 0, y - 1, z - 0},
            {x - 0, y - 0, z - 1}
        };

        bool canPlace = false;
        for (mc::BlockPos& pos : t) {
            int id = 0;
            id = lPlayer->lvl->getBlockId(pos.x, pos.y, pos.z);
            if (id != 0) canPlace = true;
        }

        auto checkCanBePlaced = [&](int _x, int _y, int _z) {
            return mc::BlockItem(mc::Block::byId(1)).mayPlace(lPlayer->lvl, {_x, _y, _z}, mc::Direction::north, lPlayer, item);
        };

        auto checkIsBlock = [&]() {
            return mc::Item::isItemABlock(item->item->getId()) && item->getCount() > 0;
        };

        auto placeBlock = [&](int _x, int _y, int _z) {
            mc_boost::shared_ptr<mc::Packet> packet = new mc::ServerboundUseItemOnPacket(
                {_x, _y, _z}, 
                mc::Direction::north, 
                mc::InteractionHand::EInteractionHand::MAIN_HAND,
                0, 0, 0
            );
            mc::Minecraft::getInstance()->getConnection(0)->send(packet);

            lPlayer->lvl->setBlock({_x, _y, _z}, mc::Block::byId(item->item->getId())->defaultBlockState(), 3, false);
            lPlayer->lvl->setData({_x, _y, _z}, item->aux, item->aux, false);
            lPlayer->swing();

            scaffold->setPos({_x, _y, _z});
        };

        if (!canPlace) {
            for (mc::BlockPos& pos : t) {
                mc::BlockPos n[6] = {
                    {pos.x + 1, pos.y + 0, pos.z + 0},
                    {pos.x + 0, pos.y + 1, pos.z + 0},
                    {pos.x + 0, pos.y + 0, pos.z + 1},
                    {pos.x - 1, pos.y - 0, pos.z - 0},
                    {pos.x - 0, pos.y - 1, pos.z - 0},
                    {pos.x - 0, pos.y - 0, pos.z - 1}
                };

                bool wasPlaced = false;
                for (mc::BlockPos& pos2 : n) {
                    int id = 0;
                    id = lPlayer->lvl->getBlockId(pos2.x, pos2.y, pos2.z);
                    if (id != 0) {
                        if (checkIsBlock() && checkCanBePlaced(pos2.x, pos2.y, pos2.z)) {
                            placeBlock(pos2.x, pos2.y, pos2.z);
                            wasPlaced = true;
                            break;
                        }
                    }
                }

                if (wasPlaced) break;
            }   
        }

        if (checkIsBlock() && canPlace && checkCanBePlaced(x, y, z)) {
            placeBlock(x, y, z);
        }
    }

    static void onDraw() {
        static uint64_t lastTime = 0;
        uint64_t currTime = mc::System::processTimeInMilliSecsu64();;
        if (lastTime == 0) {
            lastTime = currTime;
            onTick();
            return;
        }

        uint64_t minDelay = 100;
        if ((currTime - lastTime) >= minDelay) {
            lastTime = currTime;
            onTick();
        }
    }

    static void updatePos(bool before) {
        static float yaw = 0;
        static float pitch = 0;
        Scaffold* scaffold = (Scaffold*) staticScaffold;
        if (!scaffold->getModule()->getState()) return;

        mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
        if (before) {
            yaw = lPlayer->yaw;
            pitch = lPlayer->pitch;
            if (scaffold->getPos().y == -1) return;
            double diffX = mc::toFloat(scaffold->getPos().x) + 0.5f -  lPlayer->position.x;
            double diffY = mc::toFloat(scaffold->getPos().y) + 0.5f - (lPlayer->position.y + lPlayer->getEyeHeight());
            double diffZ = mc::toFloat(scaffold->getPos().z) + 0.5f -  lPlayer->position.z;
            double dist = sqrt(pow(diffX, 2) + pow(diffZ, 2));
            float _yaw =   (float)   atan2(diffZ, diffX) * (180.0 / M_PI) - 90.0f;
		    float _pitch = (float) (-atan2(diffY, dist)) * (180.0 / M_PI);

            scaffold->setPos({0, -1, 0});

            lPlayer->yaw = yaw;
            lPlayer->pitch = pitch;
        }

        if (!before) {
            lPlayer->yaw = yaw;
            lPlayer->pitch = pitch;
        }
    }

    Module* getModule() {
        return _module;
    }

    mc::BlockPos getPos() {
        return blockPos;
    }

    void setPos(mc::BlockPos pos) {
        blockPos = pos;
    }

private:
    Module* _module;

    mc::BlockPos blockPos;
};

#endif