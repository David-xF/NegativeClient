#ifndef CHEST_ESP_H
#define CHEST_ESP_H

#include "Module.h"

#include <xf/DrawHelper.h>
#include <xf/String.h>

#include <minecraft/mc.h>

struct ChestESP* staticChestESP;

class ChestESP {
public:
    ChestESP() {
        _module = new Module(L"ChestESP", Module::Type::MODULE);
        staticChestESP = this;

        color = 0x00b4ff;
        color = ((0xFF / 4) << 24) | color;

        Module* setColModule = new Module(L"Set Color [ARGB]", Module::Type::BUTTON);
        setColModule->setEvents(nullptr, nullptr, [](Module* origin) {
            mc::CInput::GetInput()->RequestKeyboard(L"", xf::String<wchar_t>::intToHexStr(staticChestESP->getColor()).c_str(), 0, 8, [](void* data, bool unk) {
                wchar_t temp[9];
                mc::CInput::GetInput()->GetText(temp, 9);
                staticChestESP->setColor(xf::String<wchar_t>::hexStrToInt(temp));
                return 0;
            }, nullptr, mc::CInput::KeyboardMode::FULL);
        });
        _module->addModuleToSettings(setColModule);
    }

    static void draw(double x, double y, double z) {
        if (!staticChestESP->getModule()->getState()) return;
        mc::GlStateManager::disableDepthTest();
        mc::GlStateManager::disableTexture();
        mc::GlStateManager::disableLighting();
        xf::GUI::DrawHelper::DisplayBox3D(x, y, z, 1, 1, 1, staticChestESP->getColor() & 0xFFFFFF, (staticChestESP->getColor() & 0xFF000000) >> 24);
        mc::GlStateManager::enableDepthTest();
    }

    void setColor(uint32_t col) {
        color = col;
    }

    uint32_t getColor() {
        return color;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
    uint32_t color;
};

#endif