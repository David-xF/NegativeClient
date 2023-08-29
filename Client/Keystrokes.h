#ifndef KEYSTROKES_H
#define KEYSTROKES_H

#include "Module.h"

#include <xf/DrawHelper.h>
#include <xf/String.h>

#include <minecraft/mc.h>

void* staticKeyStrokes;

class KeyStrokes {
public:
    KeyStrokes() {
        _module = new Module(L"KeyStrokes", Module::Type::MODULE);
        staticKeyStrokes = this;
    }

    static void draw() {
       KeyStrokes* strokes = (KeyStrokes*) staticKeyStrokes;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;

    xf::Vector<mstd::pair<uint32_t, uint64_t>> pressedButtons; // pair<button, time>
};

#endif