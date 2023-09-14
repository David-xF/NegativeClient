#ifndef CUSTOMCHAT_H
#define CUSTOMCHAT_H

#include <code/code.h>

#include "Module.h"

#include <minecraft/mc.h>
#include <xf/DrawHelper.h>

void* staticCustomChat;

class CustomChat {
public:
    struct ChatEntry {
        mstd::wstring message;
        uint64_t addTime;
    };

    CustomChat() {
        _module = new Module(L"Custom Chat", Module::Type::MODULE);
        staticCustomChat = this;

        messages = xf::Vector<ChatEntry>();
    }

    static bool addMessageS(const wchar_t* msg) {
        CustomChat* cChat = (CustomChat*) staticCustomChat;
        if (!cChat->getModule()->getState()) return true;
        cChat->addMessage(msg);
        return false;
    }

    void addMessage(const wchar_t* msg) {
        messages.push_front({msg, mc::System::processTimeInMilliSecs()});
    }

    int getSize() {
        return messages.getSize();
    }

    void pop() {
        messages.pop_back();
    }

    const ChatEntry& at(int index) {
        return messages[index];
    }

    static void drawChat() {
        CustomChat* cChat = (CustomChat*) staticCustomChat;
        if (!cChat->getModule()->getState()) return;

        uint64_t cTime = mc::System::processTimeInMilliSecs();
        float minTime = 5.0f;
        float maxTime = 2.5f;

        int popBackCount = 0;
        for (int i = 0; i < cChat->getSize(); i++) {
            if ((mc::toFloat(cTime - cChat->at(i).addTime) / 1000.0f) > (minTime + maxTime)) popBackCount++;
        }

        for (int i = 0; i < popBackCount; i++) {
            cChat->pop();
        }

        xf::GUI::DrawHelper::renderSetup();
        mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
        
        int entrys = cChat->getSize();
        if (entrys > 7) entrys = 7;
        
        if (entrys > 0) {
            int longestWidth = 0;
            for (int i = 0; i < entrys; i++) {
                int width = font->width(cChat->at(i).message);
                if (width > longestWidth) longestWidth = width;
            }

            int totalHeight = (entrys * (2 + FONT_CHAR_HEIGHT));
            xf::GUI::DrawHelper::DisplayBox2D(0, HEIGHT - 130 - totalHeight - 5, 10 + longestWidth, totalHeight + 10, 0x30, 0x888888);
        }

        for (int i = 0; i < entrys; i++) {
            float t = mc::toFloat(cTime - cChat->at(i).addTime) / 1000.0f;
            uint8_t alpha = 0xFF - ((t > minTime) ? (((t - minTime) / maxTime) * 0xFF) : 0x0);
            if (t > (minTime + maxTime)) alpha = 0x0;
            xf::GUI::DrawHelper::DisplayText(font, cChat->at(i).message, 1.0, 5, HEIGHT - 130 - (i * (2 + FONT_CHAR_HEIGHT)), (alpha << 24) | 0xFFFFFF, false);
        }
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
    xf::Vector<ChatEntry> messages;
};

#endif