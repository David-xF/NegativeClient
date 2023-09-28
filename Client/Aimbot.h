#ifndef AIMBOT_H
#define AIMBOT_H

#include <minecraft/mc.h>

#include "Module.h"
#include <xf/String.h>
#include <xf/DrawHelper.h>

void* staticAimbot;

class Aimbot {
public:
    class RangeSlider : public Slider<int> {
    public:
        RangeSlider() : Slider<int>(0, 100, 25, 1) {
            setDrawFunc([](Slider<int>* _this) -> mstd::wstring {
                return xf::String<wchar_t>::intToStr(_this->getCurrent()).c_str();
            });

            setEvents([](Slider<int>* _this) { // Left
                    if (_this->getCurrent() != _this->getMin()) {
                        _this->setCurrent(_this->getCurrent() - _this->getIncrement());
                    }
                }, [](Slider<int>* _this) { // Right
                    if (_this->getCurrent() != _this->getMax()) {
                        _this->setCurrent(_this->getCurrent() + _this->getIncrement());
                    }
                }
            );
        }
    };

    Aimbot() {
        _module = new Module(L"Aimbot", Module::Type::MODULE);
        staticAimbot = this;

        targetPlayers = true;
        targetEntities = true;

        _module->addModuleToSettings((new Module(L"Target Entities", Module::Type::MODULE))->toggleState());
        _module->addModuleToSettings((new Module(L"Target Players", Module::Type::MODULE))->toggleState());
        _module->addModuleToSettings((new Module(L"Target Spectators", Module::Type::MODULE)));

        Module* sliderMod = new Module(L"Range", Module::Type::SLIDER);
        sliderMod->setSlider(new RangeSlider());
        _module->addModuleToSettings(sliderMod);

        selected = nullptr;
    }

    static void selectEntity() {
        Aimbot* aimbot = (Aimbot*) staticAimbot;
        if (!aimbot->getModule()->getState()) return;

        mc::Minecraft* minecraft = mc::Minecraft::getInstance();
        mc::LocalPlayer* lPlayer = minecraft->thePlayer;
        mc::Level* level = lPlayer->lvl;    

        mc::Entity* nearest = nullptr;

        if (aimbot->shouldTargetPlayers()) {
            for (mc_boost::shared_ptr<mc::Player>& player : level->players) {
                if (player->uuid != lPlayer->uuid) {
                    bool isAllowedToBeAdded = true;
                    if (!aimbot->shouldTargetSpectators()) {
                        if (player->type() == mc::RemotePlayer::GetType()) {
                            mc::RemotePlayer* rPlayer = (mc::RemotePlayer*) player.ptr;
                            mc::GameType* gType = rPlayer->GetGameType();
                            if (gType->getId() == 3) isAllowedToBeAdded = false;
                        }
                    }

                    if (isAllowedToBeAdded) {
                        float currEntityDist = player->position.distance(lPlayer->position);
                        if (!nearest) {
                            nearest = player.ptr;
                        } else {
                            float nearestEntityDist = nearest->position.distance(lPlayer->position);
                            if (nearestEntityDist > currEntityDist) nearest = player.ptr;
                        }
                    }
                }
            }
        }

        if (aimbot->shouldTargetEntities()) {
            for (mc_boost::shared_ptr<mc::Entity>& entity : level->entities) {
                bool isPlayer = false;
                for (mc_boost::shared_ptr<mc::Player>& player : level->players) {
                    if (player.ptr == entity.ptr) isPlayer = true;
                }

                if (entity.ptr != lPlayer && !Module::isBlackListed(entity->type()) && !isPlayer) {
                    float currEntityDist = entity->position.distance(lPlayer->position);
                    if (!nearest) {
                        nearest = entity.ptr;
                    } else {
                        float nearestEntityDist = nearest->position.distance(lPlayer->position);
                        if (nearestEntityDist > currEntityDist) nearest = entity.ptr;
                    }
                }
            }
        }

        aimbot->setSelectedEntity(nearest);
    }

    static void aim() {
        Aimbot* aimbot = (Aimbot*) staticAimbot;
        if (!aimbot->getModule()->getState()) return;
        mc::Minecraft* minecraft = mc::Minecraft::getInstance();
        if (!minecraft) return;
        mc::LocalPlayer* lPlayer = minecraft->thePlayer;
        if (!lPlayer) return;
        mc::Level* level = lPlayer->lvl;


        bool isRight = false;
        for (int i = 0; i < 3; i++) {
            if (lPlayer->lvl == mc::Minecraft::getInstance()->getLevel(i)) isRight = true;
        }

        if (!isRight) return;

        if (!level) return;    
        mc::Entity* nearest = aimbot->getSelectedEntity();

        if (nearest) {
            mc::Item* item = mc::Item::byId(level->getBlockId(mc::toInt(lPlayer->position.x), mc::toInt(lPlayer->position.y + 0.9) - 1, mc::toInt(lPlayer->position.z)));
            if (mc::Item::isItemABlock(item->getId()) && item->getId() != 0) {
                lPlayer->jumpFromGround();
            }

            if (nearest->position.distance(lPlayer->position) >= aimbot->getMaxDistance()) return;
            double diffX =  nearest->position.x - lPlayer->position.x;
            double diffY = (nearest->position.y + nearest->getEyeHeight()) - (lPlayer->position.y + lPlayer->getEyeHeight());
            double diffZ =  nearest->position.z - lPlayer->position.z;
            double dist  =  sqrt(pow(diffX, 2) + pow(diffZ, 2));
            float yaw    = (float)   atan2(diffZ, diffX) * (180.0 / M_PI) - 90.0f;
		    float pitch  = (float) (-atan2(diffY, dist)) * (180.0 / M_PI);

            if (isnan(yaw)   || isinf(yaw))   yaw   = 0.0f;
            if (isnan(pitch) || isinf(pitch)) pitch = 0.0f;

            lPlayer->yaw = yaw;
            lPlayer->pitch = pitch;
        }

        return;
    }

    Module* getModule() {
        return _module;
    }

    bool shouldTargetSpectators() {
        return _module->getPageVector()[2]->getState();
    }

    bool shouldTargetPlayers() {
        return _module->getPageVector()[1]->getState();
    }

    bool shouldTargetEntities() {
        return _module->getPageVector()[0]->getState();
    }

    float getMaxDistance() {
        return mc::toFloat(((RangeSlider*) _module->getPageVector()[3]->getSlider())->getCurrent());
    }

    void setSelectedEntity(mc::Entity* entity) {
        selected = entity;
    }   

    mc::Entity* getSelectedEntity() {
        return selected;
    }

private:
    Module* _module;
    bool targetPlayers;
    bool targetEntities;
    mc::Entity* selected;
};

#endif