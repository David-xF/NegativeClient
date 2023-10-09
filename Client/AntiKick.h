#ifndef ANTIKICK_H
#define ANTIKICK_H

#include "Module.h"

#include <minecraft/mc.h>

struct AntiKick* staticAntiKick;

class AntiKick {
public:
    AntiKick() {
        _module = new Module(L"Anti Kick", Module::Type::MODULE);
        staticAntiKick = this;
    }

    static bool onKick(mc::DisconnectPacket::eDisconnectReason reason) {
        if (!staticAntiKick->getModule()->getState()) return true;
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