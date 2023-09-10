#ifndef KILLAURA_H
#define KILLAURA_H

#include "Module.h"
#include <minecraft/mc.h>
#include <xf/DrawHelper.h>

void* staticKillAura;

class KillAura {
public:
    KillAura() {
        _module = new Module(L"Kill Aura", Module::Type::MODULE);
        staticKillAura = this;

        _module->addModuleToSettings((new Module(L"Target Entities", Module::Type::MODULE))->toggleState());
        _module->addModuleToSettings((new Module(L"Target Players", Module::Type::MODULE))->toggleState());

        playerForInfoDisplay = nullptr;
        maxDistance = 7.0f;
    }

    static mc::Entity* selectEntity() {
        KillAura* killAura = (KillAura*) staticKillAura;
        if (!killAura->getModule()->getState()) return nullptr;

        mc::Minecraft* minecraft = mc::Minecraft::getInstance();
        mc::LocalPlayer* lPlayer = minecraft->thePlayer;
        mc::Level* level = lPlayer->lvl;    

        mc::Entity* nearest = nullptr;

        if (killAura->canTargetPlayers()) {
            for (mc_boost::shared_ptr<mc::Player>& player : level->players) {
                if (player->uuid != lPlayer->uuid) {
                    bool isAllowedToBeAdded = true;
                    if (player->type() == mc::RemotePlayer::GetType()) {
                        mc::RemotePlayer* rPlayer = (mc::RemotePlayer*) player.ptr;
                        mc::GameType* gType = rPlayer->GetGameType();
                        if (gType->getId() == 3) isAllowedToBeAdded = false;
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

        if (killAura->canTargetEntities()) {
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

        killAura->setPlayer(nullptr);
        if (nearest->type() == mc::RemotePlayer::GetType()) killAura->setPlayer((mc::Player*) nearest);
    
        return nearest;
    }

    static void onTick(bool before) {
        KillAura* killAura = (KillAura*) staticKillAura;
        if (!killAura->getModule()->getState()) return;

        mc::Minecraft* minecraft = mc::Minecraft::getInstance();
        mc::LocalPlayer* lPlayer = minecraft->thePlayer;
        
        if (!before) {
            mc::Entity* nearestEntity = selectEntity();
            if (nearestEntity) {
                if (nearestEntity->position.distance(lPlayer->position) > killAura->getMaxDist()) return;

                mc_boost::shared_ptr<mc::Packet> packet = new mc::ServerboundInteractPacket(nearestEntity);
                mc::Minecraft::getInstance()->getConnection(0)->send(packet);

                lPlayer->swing();
            }
        }
    }

    static void draw() {
        KillAura* killAura = (KillAura*) staticKillAura;
        if (!killAura->getModule()->getState()) return;
        mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
        mc::Player* target = killAura->getPlayer();

        bool isStillAvailable = false;
        for (mc_boost::shared_ptr<mc::Player>& player : lPlayer->lvl->players) {
            if (target == player.ptr) {
                isStillAvailable = true;
                break;      
            }
        }

        if (isStillAvailable) {
            if (target->position.distance(lPlayer->position) > killAura->getMaxDist()) return;
        }

        if (!isStillAvailable) return;

        auto path = mc::CMinecraftApp::getSkinPathFromId(nullptr, target->skin);
        uint32_t texture_id = mc::Minecraft::getInstance()->textures->loadMemTexture(path, 0x150);

        const float sizeX = 150;
        const float sizeY = 30;
        const float x = mc::toFloat(WIDTH / 2) + (sizeX / 2) + sizeY;
        const float y = mc::toFloat(HEIGHT / 2);
        const float cornerWidth = 8;

        xf::GUI::DrawHelper::renderSetup();
        xf::GUI::DrawHelper::drawRoundedBox(x, y, sizeX, sizeY, cornerWidth, 0, 0xFF);
        xf::GUI::DrawHelper::drawRoundedBoxImg(x - (sizeY * 2), y, 20, 20, cornerWidth / 2, texture_id, target->skinAnim, false);
        xf::GUI::DrawHelper::drawRoundedBoxImg(x - (sizeY * 2), y, 20, 20, cornerWidth / 2, texture_id, target->skinAnim, true);

        mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
        const float _x = x + (sizeX / 2) - 10;
        const float _y = y - (sizeY / 2);
        const float _sX = 28;
        const float _sY = 12;
        xf::GUI::DrawHelper::drawRoundedBox(_x, _y, _sX, _sY, 6, 0xFFFFFF, 0xFF);
        const float size = 10;
        wchar_t healthText[10];
        mc_swprintf(healthText, 10, L"%d", mc::toInt(target->getHealth() + target->getAbsorptionAmount()));
        float textWidth = font->width(healthText) / 2;
        uint32_t hpTextColor = 0xFFFF0000;
        if (target->getAbsorptionAmount() > 0) hpTextColor = 0xFFFFAA00; 
        xf::GUI::DrawHelper::DisplayText(font, healthText, 1, _x - textWidth, _y - (FONT_CHAR_HEIGHT / 2), hpTextColor, false);

        float healthPercent = target->getHealth() / target->getMaxHealth();
        float width = 108;
        float cWidth = width * healthPercent;
        float __y = y + 9;
        xf::GUI::DrawHelper::drawRoundedBox(x + 10, __y, width, 4, 2, 0x330000, 0xFF);
        if (healthPercent > 0)
            xf::GUI::DrawHelper::drawRoundedBox(x + 10 - ((width - cWidth) / 2), __y, cWidth, 4, 2, 0xFF0000, 0xFF);

        float armorPercent = target->getArmorCoverPercentage();
        cWidth = width * armorPercent;
        xf::GUI::DrawHelper::drawRoundedBox(x + 10, __y - 6, width, 4, 2, 0x222222, 0xFF);
        if (armorPercent > 0)
            xf::GUI::DrawHelper::drawRoundedBox(x + 10 - ((width - cWidth) / 2), __y - 6, cWidth, 4, 2, 0xAAAAAA, 0xFF);
        
        xf::GUI::DrawHelper::DisplayText(font, target->name, 1, x + 10 - (width / 2), y - 2 - FONT_CHAR_HEIGHT, 0xFFFFFFFF, false);
    }

    float getMaxDist() {
        return maxDistance;
    }

    void setPlayer(mc::Player* player) {
        playerForInfoDisplay = player;
    }

    mc::Player* getPlayer() {
        return playerForInfoDisplay;
    }

    Module* getModule() {
        return _module;
    }

    bool canTargetEntities() {
        return _module->getPageVector()[0]->getState();
    }

    bool canTargetPlayers() {
        return _module->getPageVector()[1]->getState();
    }

private:
    Module* _module;
    mc::Player* playerForInfoDisplay;

    float maxDistance;
};

#endif