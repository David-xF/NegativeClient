#pragma once

#include <minecraft/mc.h>

#include <xf/String.h>

class Command {
public:
    Command(const wchar_t* cmd) {
        cmdName = xf::String<wchar_t>(L"/") + cmd;
    }

    virtual void onCall(xf::Vector<xf::String<wchar_t>> args) {

    }

    xf::String<wchar_t> getName() {
        return cmdName;
    }

private:
    xf::String<wchar_t> cmdName;
};