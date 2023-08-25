#ifndef CUSTOMSKY_H
#define CUSTOMSKY_H

#include "Module.h"
#include <xf/DrawHelper.h>

#include <minecraft/mc.h>

void* staticCustomSky;

class CustomSky {
public:
    CustomSky() {
        _module = new Module(L"Custom Sky", Module::Type::MODULE);
        staticCustomSky = this;

        img_id = 0;
    }

    static bool draw() {
        CustomSky* sky = (CustomSky*) staticCustomSky;
        int img_id = sky->getImageID();
        
        if (!sky->getModule()->getState()) return true;

        if (img_id == 0) {
            mc::BufferedImage* image = new mc::BufferedImage(L"/misc/Sky.png", false, false, L"Common/", 2);
            img_id = mc::GlStateManager::genTexture();
            mc::Minecraft::getInstance()->textures->loadTexture(image, img_id);
            sky->setImageID(img_id);
        }

        mc::GlStateManager::translatef(0.0f, 0.0f, 0.0f);
        mc::GlStateManager::scalef(1.0f, 1.0f, 1.0f);
        mc::GlStateManager::disableCull();
        mc::GlStateManager::disableLighting();
        mc::GlStateManager::disableDepthTest();
        mc::GlStateManager::enableBlend();
        mc::GlStateManager::blendFunc(4, 5);
        mc::GlStateManager::color4f(1, 1, 1, 1);
        mc::GlStateManager::disableFog();
        mc::GlStateManager::color4f(1, 1, 1, 1);
        if (img_id != 0) xf::GUI::DrawHelper::DisplayBox3DImg(-250, -250, -250, 500, 500, 500, img_id);
        else             xf::GUI::DrawHelper::DisplayBox3D(-250, -250, -250, 500, 500, 500, 0x0, 0xFF);
        mc::GlStateManager::enableDepthTest();
        mc::GlStateManager::disableBlend();
        mc::GlStateManager::enableCull();

        return false;
    }

    Module* getModule() {
        return _module;
    }

    void setImageID(int id) {
        img_id = id;
    }

    int getImageID() {
        return img_id;
    }

    void reloadImg() {
        mc::Minecraft::getInstance()->textures->releaseTexture(img_id);
        img_id = 0;
    }

private:
    Module* _module;
    int img_id;
};

#endif