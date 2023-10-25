#ifndef ESP_H
#define ESP_H

#include "Module.h"

#include <xf/DrawHelper.h>
#include <xf/String.h>

#include <minecraft/mc.h>

struct ESP* staticESP;

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
            mc::CInput::GetInput()->RequestKeyboard(L"", xf::String<wchar_t>::intToHexStr(staticESP->getColor()).c_str(), 0, 8, [](void* data, bool unk) {
                wchar_t temp[9];
                mc::CInput::GetInput()->GetText(temp, 9);
                staticESP->setColor(xf::String<wchar_t>::hexStrToInt(temp));
                return 0;
            }, nullptr, 0);
        });
        _module->addModuleToSettings(setColModule);
        _module->addModuleToSettings(new Module(L"Tracers", Module::Type::MODULE));
    }

    static void draw(void* renderer, const mc_boost::shared_ptr<mc::Entity>& ref, uint32_t unk, float x, float y, float z, float a, float b) {
        if (!staticESP->getModule()->getState()) return; 
        
        mc::Player* player = nullptr;
        for (mc_boost::shared_ptr<mc::Player>& _player : ref->lvl->players) {
            if (_player.get() == ref.get()) {
                player = _player.get();
                break;
            }
        }

        bool shouldDraw = false;
        if (player && staticESP->shouldTargetPlayers())   shouldDraw = true;
        if (!player && staticESP->shouldTargetEntities()) shouldDraw = true;

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

        uint32_t color = !player ? staticESP->getColor() : colors[player->colourIndex];
        if (!staticESP->playerColor()) color = staticESP->getColor();
        if (staticESP->playerColor() && player) color = (staticESP->getColor() & 0xFF000000) | color;
        
        // My Hitbox
        xf::GUI::DrawHelper::DisplayBox3D(
            x - sX,  y, z - sZ,
            sX * 2, sY, sZ * 2,
            color & 0xFFFFFF, (color & 0xFF000000) >> 24
        );

        if (staticESP->shouldDrawTracers()) {
            mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
            mc::GlStateManager::disableTexture();
            mc::GlStateManager::lineWidth(5);
            mc::GlStateManager::pushMatrix();
            builder->begin(MC_GL_LINES);
            builder->color(0, 0xFF);
            builder->vertex(0, 0, 0);
            builder->vertex(x, y, z);
            builder->end();
            mc::GlStateManager::popMatrix();
        }

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

    bool shouldDrawTracers() {
        return _module->getPageVector()[4]->getState();
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