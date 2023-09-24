#ifndef SEENAMETAGS_H
#define SEENAMETAGS_H

#include "Module.h"

#include <minecraft/mc.h>

void* staticSeeNameTags;

class SeeNameTags {
public:
    SeeNameTags() {
        _module = new Module(L"See Name Tags", Module::Type::MODULE);
        staticSeeNameTags = this;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif