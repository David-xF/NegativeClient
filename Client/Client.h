#ifndef CLIENT_H
#define CLIENT_H

#include <minecraft/mc.h>

#include <memory>

#include <xf/DrawHelper.h>
#include <xf/Vector.h>

#include "Module.h"

void* staticClient;

template<typename... Args>
void mc__printf(const wchar_t* x, Args... args) {
    if (mc::Minecraft::getInstance()->thePlayer == nullptr) return;
    wchar_t temp[0x80];
    mc_swprintf(temp, 0x80, x, args...);
    mc::Minecraft::getInstance()->gui->addMessage(temp, 0, 0, 0, 0, 0);
}

class Client {
public:
    Client(const wchar_t* name) {
        const int maxNameSize = 0x20;
        clientName = new wchar_t[maxNameSize];
        memcpy(clientName, name, maxNameSize);

        staticClient = this;
        mod = new Module(L"Client", Module::Type::PAGE);
        fps = 0;

        showClientMenu = false;
    }

    void addPage(Module* page) {
        mod->addModuleToVector(page);
    }

    static void draw(bool inMenu) {
        Client* client = Client::getInstance();
        client->fpsCounter();
        
        xf::GUI::DrawHelper::renderSetup();
        mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
        xf::GUI::DrawHelper::DisplayText(font, client->getName(), 1.5, 5, 5);
        wchar_t fpsCounterText[0x20];
        mc_swprintf(fpsCounterText, 0x20, L"FPS: %d", client->getFPS());
        xf::GUI::DrawHelper::DisplayText(font, fpsCounterText, 1, 5, HEIGHT - FONT_CHAR_HEIGHT - 5);

        if (inMenu) return;

        START_BUTTONCHECK(buttons & VPAD_BUTTON_L && buttons & VPAD_BUTTON_R, client->toggle(), buttons)
        END_BUTTONCHECK()
        
        client->drawMenu();
    }

    Module* getCurrentModule(Module* curr) {
        if (curr->getPageIndex() == -1) {
            return curr;
        } else {
            return getCurrentModule(curr->getPageVector()[curr->getPageIndex()]);
        }
    }

    void drawMenu() {
        if (!showClientMenu) {
            mod->reset();
            return;
        }

        START_BUTTONCHECK(buttons & VPAD_BUTTON_DOWN, mod->down(), buttons)
        ADD_BUTTONCHECK(buttons & VPAD_BUTTON_UP, mod->up())
        ADD_BUTTONCHECK(buttons & VPAD_BUTTON_RIGHT, mod->right())
        ADD_BUTTONCHECK(buttons & VPAD_BUTTON_LEFT, mod->left())
        END_BUTTONCHECK()

        const float x = 25;
        const float y = 25;
        const float sizeX = 50 * 2;
        const float sizeY = 75 * 2;

        const float space = 2.0f; // Space Between two Modules;
        const float angle = 10.0f;

        const uint32_t bgColor = 0;

        const float markerWidth = 3.0f;
        const float textSpace = 4.0f;

        const float outlineWidth = 2.0f;

        mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
        //xf::GUI::DrawHelper::DisplayBox2D(x, y, sizeX, sizeY, bgColor, 0xFF);
        xf::GUI::DrawHelper::drawRoundedBox(x + (sizeX / 2), y + (sizeY / 2), sizeX + outlineWidth, sizeY + outlineWidth, angle, 0xFFFFFF, 0xFF);
        xf::GUI::DrawHelper::drawRoundedBox(x + (sizeX / 2), y + (sizeY / 2), sizeX, sizeY, angle, bgColor, 0xFF);
        float cNameWidth = font->width(clientName) / 2;
        xf::GUI::DrawHelper::DisplayText(font, clientName, 1, (sizeX / 2) + x - cNameWidth, y + 5);
        
        auto drawMod = [&](Module* mod, int index, int _selectIndex) {
            float yOffset = index * mc::toFloat(FONT_CHAR_HEIGHT + space);
            
            xf::GUI::DrawHelper::DisplayBox2D(x, y + 23 + yOffset - space, markerWidth, FONT_CHAR_HEIGHT + space, mod->getColor(), 0xFF);
            if (index == _selectIndex) {
                xf::GUI::DrawHelper::DisplayBox2D(
                    x + markerWidth, y + 23 + yOffset - space, sizeX - markerWidth, FONT_CHAR_HEIGHT + space, 0xFFFFFF, 0x20
                );
            }

            xf::GUI::DrawHelper::DisplayText(font, mod->getName(), 1, x + textSpace + markerWidth, y + 23 + yOffset, 0xFFFFFFFF, false);  
        };
        
        if (mod->getPageIndex() == -1) {
            for (int i = 0; i < mod->getPageVector().getSize(); i++) {
                drawMod(mod->getPageVector()[i], i, mod->selctedInt());
            }
        } else {    
            Module* currMod = getCurrentModule(mod);
            for (int i = 0; i < currMod->getPageVector().getSize(); i++) {
                drawMod(currMod->getPageVector()[i], i, currMod->selctedInt());
            }
        }
    }

    void fpsCounter() {
        static int count = 0;
        static uint64_t basetime = 0;
        if (count == 0) {
            basetime = mc::System::processTimeInMilliSecsu64();
        } else {
            uint64_t now = mc::System::processTimeInMilliSecsu64();
            if (now - basetime >= 1000) {
                fps = mc::toInt(mc::toFloat(count * 1000) / mc::toFloat(now - basetime) + 0.5f);
                basetime = now;
                count = 0;
            }
        }
        count++;
    }

    const wchar_t* getName() {
        return clientName;
    }

    int getFPS() {
        return fps;
    }

    static Client* getInstance() {
        return (Client*) staticClient;
    }

    void toggle() {
        showClientMenu = !showClientMenu;
    }

private:
    wchar_t* clientName;
    int fps;
    Module* mod;

    bool showClientMenu;
};

#endif