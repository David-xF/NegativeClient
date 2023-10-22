#ifndef CLIENT_H
#define CLIENT_H

#include <minecraft/mc.h>

#include <memory>

#include <xf/DrawHelper.h>
#include <xf/Vector.h>

#include "Module.h"

struct Client* staticClient;

class Client {
public:
    Client(const wchar_t* name) {
        clientName = new wchar_t[mc_wcslen(name) + 1];
        mc_wcscpy(clientName, name);

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

        auto pow = [](int a, int b) -> int {
            int c = a;
            for (int d = 1; d < b; d++) {
                c = c * a;
            }

            return c;
        };

        auto getBytes = [pow](uint32_t bytes) -> char* {
            if (bytes >= pow(10, 9)) {
                return "gb";
            } else if (bytes >= pow(10, 6)) {
                return "mb";
            } else if (bytes >= pow(10, 3)) {
                return "kb";
            } else {
                return "b";
            }
        };

        auto fix = [pow](uint32_t bytes) -> int {
            if (bytes > pow(10, 9)) {
                return bytes / pow(10, 9);
            } else if (bytes > pow(10, 6)) {
                return bytes / pow(10, 6);
            } else if (bytes > pow(10, 3)) {
                return bytes / pow(10, 3);
            } else {
                return bytes;
            }
        };
        
        xf::GUI::DrawHelper::renderSetup();
        mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
        xf::GUI::DrawHelper::DisplayText(font, client->getName(), 1.5, 5, 5);
        wchar_t fpsCounterText[0x70];
        bool debugInfo = false;
        if (debugInfo) {
            mc_swprintf(fpsCounterText, 0x70, L"Build: %s, %s  |  FPS: %d  |  Total Memory: [%d %s  |  %d  |  %d  |  %d  |  %d]", __DATE__, __TIME__, client->getFPS(), fix(totalMemoryAllocated), getBytes(totalMemoryAllocated), totalAllocs, totalDeletes, totalAllocs - totalDeletes, fakeDeletes);
        } else {
            mc_swprintf(fpsCounterText, 0x70, L"Build: %s, %s  |  FPS: %d", __DATE__, __TIME__, client->getFPS());
        }
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

        auto openSettings = [&](bool isEnd, uint64_t &t1, uint64_t &t2) {
            static uint32_t lastBtnPressed = 0;
            Module* currMod = getCurrentModule(mod);
            if (currMod->getPageVector()[currMod->selctedInt()]->hasSettings()) {
                if (lastBtnPressed == 1 && isEnd) {
                    lastBtnPressed = 0;
                    mod->right();
                } else if (lastBtnPressed == 0 && !isEnd) {
                    lastBtnPressed = 1;
                } else if (lastBtnPressed == 1 && !isEnd) {
                    currMod->openSettings();
                    t1 = mc::System::processTimeInMilliSecs();
                    t2 = mc::System::processTimeInMilliSecs();
                    lastBtnPressed = 2;
                } else if (lastBtnPressed == 2 && isEnd) {
                    lastBtnPressed = 0;
                }
            } else if (!isEnd) {
                mod->right();
            }
        };

        START_TIMED_BUTTONCHECK(buttons & VPAD_BUTTON_DOWN, mod->down(), buttons, 300, 20)
        ADD_TIMED_BUTTONCHECK(buttons & VPAD_BUTTON_UP,     mod->up())
        ADD_TIMED_BUTTONCHECK(buttons & VPAD_BUTTON_RIGHT,  openSettings(false, lastClickTime, lastTime))
        ADD_TIMED_BUTTONCHECK(buttons & VPAD_BUTTON_LEFT,   if (!mod->left()) toggle())
        END_TIMED_BUTTONCHECK(openSettings(true, lastClickTime, lastTime))

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
        const wchar_t* dName = getCurrentModule(mod)->getName();
        float cNameWidth = font->width(dName) / 2;
        xf::GUI::DrawHelper::DisplayText(font, dName, 1, (sizeX / 2) + x - cNameWidth, y + 5);
        
        auto drawMod = [&](Module* mod, int index, int _selectIndex) {
            float yOffset = index * mc::toFloat(FONT_CHAR_HEIGHT + space);
            
            xf::GUI::DrawHelper::DisplayBox2D(x, y + 23 + yOffset - space, markerWidth, FONT_CHAR_HEIGHT + space, mod->getColor(), 0xFF);
            if (index == _selectIndex) {
                xf::GUI::DrawHelper::DisplayBox2D(
                    x + markerWidth, y + 23 + yOffset - space, sizeX - markerWidth, FONT_CHAR_HEIGHT + space, 0xFFFFFF, 0x20
                );
            }

            if (mod->getPageVector().getSize() > 0 || mod->isSlider()) {
                mstd::wstring indicator = L"";
                if (mod->isSlider()) {
                    wchar_t t[0x40];
                    Slider<>* slider = (Slider<>*) mod->getSlider();
                    mstd::wstring(*drawFunc)(Slider<>*) = (mstd::wstring(*)(Slider<>*)) slider->getDrawFunc();
                    mc_swprintf(t, 0x40, L"< %ls >", drawFunc(slider).c_str());
                    indicator = t;    
                }
                else if (mod->hasSettings()) indicator = L"[S]";
                else                         indicator = L"[+]";
                xf::GUI::DrawHelper::DisplayText(font, indicator, 1, x + sizeX - font->width(indicator) - 4, y + 23 + yOffset, 0xFFFFFFFF, false);
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

    // By InuPong
    void fpsCounter() {
        static int count = 0;
        static uint64_t basetime = 0;
        if (count == 0) {
            basetime = mc::System::processTimeInMilliSecs();
        } else {
            uint64_t now = mc::System::processTimeInMilliSecs();
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