#ifndef XRAY_H
#define XRAY_H

#include "Module.h"

#include <minecraft/mc.h>

void* staticXRay;

// Not FINISHED, Will continue Later
class XRay {
public:
    XRay(void* _realTesselate) {
        _module = new Module(L"X-Ray", Module::Type::MODULE);
        staticXRay = this;

        _module->setEvents(onToggle, onToggle, nullptr);
        realTesselate = (void(*)(struct BlockRenderer*, struct BlockState*, mc::Level*, const mc::BlockPos&)) _realTesselate; 
    }

    static void onToggle(Module* _this) {
        // Do later to reload all Chunks
    }

    static void tesselateBlock(struct BlockRenderer* _this, struct BlockState* state, mc::Level* level, const mc::BlockPos& pos) {
        XRay* xray = (XRay*) staticXRay;
        if (!xray->getModule()->getState()) return xray->renderBlock(_this, state, level, pos);

        int blockId = level->getBlockId(pos.x, pos.y, pos.z);
        if (blockId == 14) {
            return xray->renderBlock(_this, state, level, pos); // 63 To render all sides
        }

        return;
    }

    void renderBlock(struct BlockRenderer* _this, struct BlockState* state, mc::Level* level, const mc::BlockPos& pos) {
        realTesselate(_this, state, level, pos);
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
    void(*realTesselate)(struct BlockRenderer*, struct BlockState*, mc::Level*, const mc::BlockPos&);
};

#endif