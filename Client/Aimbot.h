#ifndef AIMBOT_H
#define AIMBOT_H

#include <minecraft/mc.h>

#include "Module.h"
#include <xf/DrawHelper.h>

void* staticAimbot;

class Aimbot {
public:
    Aimbot() {
        _module = new Module(L"Aimbot", Module::Type::MODULE);
        staticAimbot = this;

        targetPlayers = true;
        targetEntities = true;
        maxDistance = 25.0f;

        _module->addModuleToSettings((new Module(L"Target Entities", Module::Type::MODULE))->toggleState());
        _module->addModuleToSettings((new Module(L"Target Players", Module::Type::MODULE))->toggleState());
    }

    static void aim() {
        Aimbot* aimbot = (Aimbot*) staticAimbot;
        if (!aimbot->getModule()->getState()) return;
        mc::Entity* nearest = nullptr;
        mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
        mc::Level* level = lPlayer->lvl;

        if (aimbot->shouldTargetPlayers()) {
            for (mc_boost::shared_ptr<mc::Player>& player : level->players) {
                if (player->uuid != lPlayer->uuid) {
                    float currEntityDist = player->position.distance(lPlayer->position);
                    if (nearest == nullptr) {
                        nearest = player.ptr;
                    } else {
                        float nearestEntityDist = nearest->position.distance(lPlayer->position);
                        if (nearestEntityDist > currEntityDist) nearest = player.ptr;
                    }
                }
            }
        }

        if (aimbot->shouldTargetEntities()) {
            for (mc_boost::shared_ptr<mc::Entity>& entity : level->entities) {
                if (entity.ptr != lPlayer && !Module::isBlackListed(entity->type())) {
                    float currEntityDist = entity->position.distance(lPlayer->position);
                    if (nearest == nullptr) {
                        nearest = entity.ptr;
                    } else {
                        float nearestEntityDist = nearest->position.distance(lPlayer->position);
                        if (nearestEntityDist > currEntityDist) nearest = entity.ptr;
                    }
                }
            }
        }

        if (nearest != nullptr) {
            if (nearest->position.distance(lPlayer->position) >= aimbot->getMaxDistance()) return;
            double diffX = nearest->position.x - lPlayer->position.x;
            double diffY = (nearest->position.y + nearest->getEyeHeight()) - (lPlayer->position.y + lPlayer->getEyeHeight());
            double diffZ = nearest->position.z - lPlayer->position.z;
            double dist = sqrt(pow(diffX, 2) + pow(diffZ, 2));
            float yaw =   (float)   atan2(diffZ, diffX) * (180.0 / M_PI) - 90.0f;
		    float pitch = (float) (-atan2(diffY, dist)) * (180.0 / M_PI);

            lPlayer->yaw = yaw;
            lPlayer->pitch = pitch;
        }
    }

    Module* getModule() {
        return _module;
    }

    bool shouldTargetPlayers() {
        return _module->getPageVector()[1]->getState();
    }

    bool shouldTargetEntities() {
        return _module->getPageVector()[0]->getState();
    }

    float getMaxDistance() {
        return maxDistance;
    }

private:
    Module* _module;
    bool targetPlayers;
    bool targetEntities;
    float maxDistance;
};

#endif