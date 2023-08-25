#ifndef ESP_H
#define ESP_H

#include "Module.h"

#include <xf/DrawHelper.h>

#include <minecraft/mc.h>

void* staticESP;

class ESP {
public:
    ESP() {
        _module = new Module(L"ESP", Module::Type::MODULE);
        staticESP = this;
    }

    static void draw(void* renderer, const mc_boost::shared_ptr<mc::Entity>& ref, uint32_t unk, float x, float y, float z, float a, float b) {
        mc::Player* player = nullptr;
        for (mc_boost::shared_ptr<mc::Player>& _player : ref->lvl->players) {
            if (_player.ptr == ref.ptr) {
                player = _player.ptr;
                break;
            }
        }

        code::Func<void, 0x0317a08c>()(); // Lighting::turnOn(void)
        //if (esp->playerOnly && player == nullptr) return;

        mc::GlStateManager::disableTexture();
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
        uint32_t color = player == nullptr ? 0x00b4ff : colors[player->colourIndex];
        // My Hitbox
        xf::GUI::DrawHelper::DisplayBox3D(
            x - sX,  y, z - sZ,
            sX * 2, sY, sZ * 2,
            color, 0xFF / 4
        );

        mc::GlStateManager::disableBlend();
        mc::GlStateManager::enableDepthTest();
        code::Func<void, 0x0317a08c>()(); // Lighting::turnOn(void)
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif