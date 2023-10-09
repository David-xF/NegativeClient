#ifndef WORLDEDIT_H
#define WORLDEDIT_H

#include "Module.h"

#include <minecraft/mc.h>

struct WorldEdit* staticWorldEdit;

class WorldEdit {
public:
    WorldEdit() {
        _module = new Module(L"WorldEdit", Module::Type::MODULE);
        staticWorldEdit = this;
    }

    static void render3D() {
        
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif