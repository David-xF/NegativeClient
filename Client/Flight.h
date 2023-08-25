#ifndef FLIGHT_H
#define FLIGHT_H

#include "Module.h"
#include <xf/DrawHelper.h>

#include <minecraft/mc.h>

void* staticFlight;

class Flight {
public:
    Flight() {
        _module = new Module(L"Flight", Module::Type::MODULE);
        staticFlight = this;
    }

    static void onTick(bool before) {
        if (!before) return;

        Flight* mod = (Flight*) staticFlight;

        bool canFly = mod->getModule()->getState();
        mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
        int gamemode = lPlayer->GetGameType()->getId();
        if (gamemode == 1 || gamemode == 3) canFly = true;
        if (lPlayer->isAllowedToFly()) canFly = true;

        lPlayer->abilities.canFly = canFly;
        if (gamemode == 3) lPlayer->abilities.isFlying = true;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif