#ifndef KEYSTROKES_H
#define KEYSTROKES_H

#include "Module.h"

#include <exports/vpad_functions.h>

#include <xf/DrawHelper.h>
#include <xf/String.h>

#include <minecraft/mc.h>

void* staticKeyStrokes;

class KeyStrokes {
public:
    KeyStrokes() {
        _module = new Module(L"KeyStrokes", Module::Type::MODULE);
        staticKeyStrokes = this;

        pressedButtons = xf::Vector<mstd::pair<uint32_t, float>>();
    }

    void drawKey(int x, int y, uint32_t key, float delta, int index) {
        float width = 10.0f;
        const float maxRadius = sqrtf(powf(width, 2) + powf(width, 2)) + 0.5f;

        int vecLength = getPressedButtons().getSize();
        for (int i = 0; i < vecLength; i++) {
            mstd::pair<uint32_t, float> pair = getPressedButtons().front();

            if (maxRadius > pair.t2) {
                getPressedButtons().push_back(getPressedButtons().front());
                getPressedButtons().pop_front();
            } else {
                getPressedButtons().pop_front();
            }
        }

        const float expandSpeed = maxRadius;
        for (int i = 0; i < getPressedButtons().getSize(); i++) {
            getPressedButtons()[i].t2 += expandSpeed * delta;
        }

        mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
        xf::GUI::DrawHelper::renderSetup();
        xf::GUI::DrawHelper::DisplayBox2D(x - width, y - width, width * 2, width * 2, 0x0, 0xFF);

        const int segments = 20;
        mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
        mc::GlStateManager::disableTexture();
        mc::GlStateManager::pushMatrix();
        mc::GlStateManager::color4f(1, 1, 1, 1);
        builder->begin(0);

        float prec = getPressedButtons()[index].t2 / maxRadius;
        if (prec > 1.0f) prec = 1.0f;

        uint8_t col = 255 - mc::toInt(255.0f * prec);
        uint32_t color = (col << 16) | (col << 8) | (col << 0);

        builder->color(color, 0xFF);
        for (int j = 0; j < segments; j++) {
            float deg = 360.0f / mc::toFloat(segments);
            float currx = sinf(degToRad(deg * j)) * getPressedButtons()[index].t2;
            float curry = cosf(degToRad(deg * j)) * getPressedButtons()[index].t2;
            float nextx = sinf(degToRad(deg * (j + 1))) * getPressedButtons()[index].t2;
            float nexty = cosf(degToRad(deg * (j + 1))) * getPressedButtons()[index].t2;

            if (currx > width) currx = width;
            if (curry > width) curry = width;
            if (nextx > width) nextx = width;
            if (nexty > width) nexty = width;
            if (-width > currx) currx = -width;
            if (-width > curry) curry = -width;
            if (-width > nextx) nextx = -width;
            if (-width > nexty) nexty = -width;

            builder->vertex(x, y);
            builder->vertex(currx + x, curry + y);
            builder->vertex(nextx + x, nexty + y);
        }

        builder->end();
        mc::GlStateManager::popMatrix();

        wchar_t* t = L"";
        switch (key) {
        case VPAD_BUTTON_ZR:
            t = L"ZR";
            break;
        case VPAD_BUTTON_ZL:
            t = L"ZL";
            break;
        case VPAD_BUTTON_A:
            t = L"A";
            break;
        default:
            t = L"NULL";
            break;
        }
        xf::GUI::DrawHelper::DisplayText(font, t, 1, x - (font->width(t) / 2), y - (FONT_CHAR_HEIGHT / 2), 0xFFFFFFFF, false);
    }

    static void draw() {
        KeyStrokes* strokes = (KeyStrokes*) staticKeyStrokes;
        if (!strokes->getModule()->getState()) return;

        uint64_t currTime = mc::System::processTimeInMilliSecs();
        static uint64_t lastTime = currTime;
        float delta = mc::toFloat(currTime - lastTime) / 1000.0f;
        START_BUTTONCHECK(btn & VPAD_BUTTON_ZL, strokes->getPressedButtons().push_back({VPAD_BUTTON_ZL, 0.0f}), btn)
        END_BUTTONCHECK()

        for (int i = 0; i < strokes->getPressedButtons().getSize(); i++) {
            strokes->drawKey(WIDTH / 2, HEIGHT / 2, strokes->getPressedButtons()[i].t1, delta, i);
        }

        lastTime = currTime;
    }

    xf::Vector<mstd::pair<uint32_t, float>>& getPressedButtons() {
        return pressedButtons;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;

    xf::Vector<mstd::pair<uint32_t, float>> pressedButtons; // pair<button, radius>
};

#endif