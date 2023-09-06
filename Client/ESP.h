#ifndef ESP_H
#define ESP_H

#include "Module.h"

#include <xf/DrawHelper.h>
#include <xf/String.h>

#include <minecraft/mc.h>

void* staticESP;

class ESP {
public:
    ESP() {
        _module = new Module(L"ESP", Module::Type::MODULE);
        staticESP = this;

        color = 0x00b4ff;
        color = ((0xFF / 4) << 24) | color;

        _module->addModuleToSettings((new Module(L"Target Entities", Module::Type::MODULE))->toggleState());
        _module->addModuleToSettings((new Module(L"Target Players", Module::Type::MODULE))->toggleState());
        _module->addModuleToSettings((new Module(L"Player Color", Module::Type::MODULE))->toggleState());
        Module* setColModule = new Module(L"Set Color [ARGB]", Module::Type::BUTTON);
        setColModule->setEvents(nullptr, nullptr, [](Module* origin) {
            ESP* esp = (ESP*) staticESP;
            mc::CInput::GetInput()->RequestKeyboard(L"", xf::String<wchar_t>::intToHexStr(esp->getColor()).c_str(), 0, 8, [](void* data, bool unk) {
                ESP* esp = (ESP*) data;
                wchar_t temp[9];
                mc::CInput::GetInput()->GetText(temp, 9);
                esp->setColor(xf::String<wchar_t>::hexStrToInt(temp));
                return 0;
            }, esp, 0);
        });
        _module->addModuleToSettings(setColModule);
    }

    static void draw(void* renderer, const mc_boost::shared_ptr<mc::Entity>& ref, uint32_t unk, float x, float y, float z, float a, float b) {
        ESP* esp = (ESP*) staticESP;
        if (!esp->getModule()->getState()) return; 
        
        mc::Player* player = nullptr;
        for (mc_boost::shared_ptr<mc::Player>& _player : ref->lvl->players) {
            if (_player.ptr == ref.ptr) {
                player = _player.ptr;
                break;
            }
        }

        bool shouldDraw = false;
        if (player && esp->shouldTargetPlayers())   shouldDraw = true;
        if (!player && esp->shouldTargetEntities()) shouldDraw = true;

        code::Func<void, 0x0317a08c>()(); // Lighting::turnOn(void)
        mc::GlStateManager::disableTexture();

        if (!shouldDraw) return;

        code::Func<void, 0x03181508>()(); // Lighting::turnOff(void)
        mc::GlStateManager::disableCull();
        mc::GlStateManager::disableDepthTest();
        mc::GlStateManager::enableBlend();
        mc::GlStateManager::blendFunc(4, 5);

        mc::GlStateManager::color4f(1, 1, 1, 1);

        const uint32_t colors[8] = { 0xFFFFFF, 0x00FF4C, 0xFF2119, 0x6385FF, 
                                     0xFF63D9, 0xFF9C00, 0xFFFB19, 0x63FFE4  };

        float sX = (ref->aabb->max.x - ref->aabb->min.x) / 2.0f;
        float sY = (ref->aabb->max.y - ref->aabb->min.y);
        float sZ = (ref->aabb->max.z - ref->aabb->min.z) / 2.0f;

        uint32_t color = !player ? esp->getColor() : colors[player->colourIndex];
        if (!esp->playerColor()) color = esp->getColor();
        if (esp->playerColor() && player) color = (esp->getColor() & 0xFF000000) | color;
        
        // My Hitbox
        xf::GUI::DrawHelper::DisplayBox3D(
            x - sX,  y, z - sZ,
            sX * 2, sY, sZ * 2,
            color & 0xFFFFFF, (color & 0xFF000000) >> 24
        );

        mc::GlStateManager::disableBlend();
        mc::GlStateManager::enableDepthTest();
        code::Func<void, 0x0317a08c>()(); // Lighting::turnOn(void)
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

    bool playerColor() {
        return _module->getPageVector()[2]->getState();
    }

    void setColor(uint32_t col) {
        color = col;
    }

    uint32_t getColor() {
        return color;
    }

private:
    Module* _module;
    uint32_t color;
};

#endif