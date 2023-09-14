#ifndef HEALTHINDICATOR_H
#define HEALTHINDICATOR_H

#include "Module.h"

#include <minecraft/mc.h>

void* staticHealthIndicator;

class HealthIndicator {
public:
    HealthIndicator() {
        _module = new Module(L"Health Indicator", Module::Type::MODULE);
        staticHealthIndicator = this;
    }

    static const wchar_t* displayName(const mstd::wstring& name) {
        HealthIndicator* healthIndicator = (HealthIndicator*) staticHealthIndicator;
        if (!healthIndicator->getModule()->getState()) return name.c_str();

        mc_boost::shared_ptr<mc::Player> player;
        mc::Minecraft::getInstance()->getLevel(0)->getPlayerByName(player, name);

        if (!player.ptr) return name.c_str();
    
        wchar_t* temp = new wchar_t[100];
        mc_swprintf(temp, 100, L"%ls [%s]", name.c_str(), toCStr(player->getHealth(), 2));
        
        return temp;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif