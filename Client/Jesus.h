#ifndef JESUS_H
#define JESUS_H

#include <code/code.h>

#include "Module.h"

#include <minecraft/mc.h>

void* staticJesus;

class Jesus {
public:
    Jesus() {
        _module = new Module(L"Jesus", Module::Type::MODULE);
        staticJesus = this;

        overridePtr = new uint32_t[1];
    }

    static void onTick() {
        uint32_t blockCollision = code::Mem(0x104D774C).as<uint32_t>();
        uint32_t waterCollision = code::Mem(0x1005941C).as<uint32_t>();

        Jesus* jesus = (Jesus*) staticJesus;
        jesus->getPtr()[0] = (jesus->getModule()->getState() ? blockCollision : waterCollision);
    }

    uint32_t* getPtr() {
        return overridePtr;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
    uint32_t* overridePtr;
};

#endif