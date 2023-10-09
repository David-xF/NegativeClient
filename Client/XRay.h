#ifndef XRAY_H
#define XRAY_H

#include "Module.h"

#include <minecraft/mc.h>

struct XRay* staticXRay;

// Not FINISHED, Will continue Later
class XRay {
public:
    XRay(void* _realTesselate) {
        _module = new Module(L"X-Ray", Module::Type::MODULE);
        staticXRay = this;

        _module->setEvents(onToggle, onToggle, nullptr);
        realTesselate = (void(*)(void* _this, void* state, const mc::BlockPos& pos, float unk_0, float unk_1, float unk_2, int unk_3, bool unk_4)) _realTesselate; 
    }

    static void onToggle(Module* _this) {
        // Do later to reload all Chunks
    }

    static void tesselateBlock(void* _this, void* state, const mc::BlockPos& pos, float unk_0, float unk_1, float unk_2, int unk_3, bool unk_4) {
        if (!staticXRay->getModule()->getState()) return staticXRay->renderBlock(_this, state, pos, unk_0, unk_1, unk_2, unk_3, unk_4);

        int blockId = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockId(pos.x, pos.y, pos.z);
        if (blockId == 14) {
            return staticXRay->renderBlock(_this, state, pos, unk_0, unk_1, unk_2, 63, unk_4); // 63 To render all sides
        }

        return;
    }

    void renderBlock(void* _this, void* state, const mc::BlockPos& pos, float unk_0, float unk_1, float unk_2, int unk_3, bool unk_4) {
        realTesselate(_this, state, pos, unk_0, unk_1, unk_2, unk_3, unk_4);
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
    void(*realTesselate)(void* _this, void* state, const mc::BlockPos& pos, float unk_0, float unk_1, float unk_2, int unk_3, bool unk_4);
};

#endif