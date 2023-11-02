#pragma once

#include "Command.h"

class TestCommand : public Command {
public:
    TestCommand() : Command(L"Test") {

    }

    void onCall(xf::Vector<xf::String<wchar_t>> args) override {
        mc_printf(L"Test Command was Called");
    }

private:
};