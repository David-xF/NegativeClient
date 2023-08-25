#ifndef ANTIKICK_H
#define ANTIKICK_H

#include "Module.h"

#include <minecraft/mc.h>

void* staticAntiKick;

template<typename... Args>
void mc_printf(const wchar_t* x, Args... args) {
    if (mc::Minecraft::getInstance()->thePlayer == nullptr) return;
    wchar_t temp[0x80];
    mc_swprintf(temp, 0x80, x, args...);
    mc::Minecraft::getInstance()->gui->addMessage(temp, 0, 0, 0, 0, 0);
}

class AntiKick {
public:
    AntiKick() {
        _module = new Module(L"Anti Kick", Module::Type::MODULE);
        staticAntiKick = this;
    }

    static bool onKick(mc::DisconnectPacket::eDisconnectReason reason) {
        AntiKick* antiKick = (AntiKick*) staticAntiKick;
        if (!antiKick->getModule()->getState()) return true;
        if (reason == mc::DisconnectPacket::eDisconnectReason::CONNECTION_LOST) return true;
        mc_printf(L"[AntiKick] Dismissed DisconnectPacket");
        return false;  
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif