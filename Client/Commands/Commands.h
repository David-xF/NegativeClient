#pragma once

#include <minecraft/mc.h>

#include "Command.h"
#include "TestCommand.h"

class Commands {
public:
    Commands() {
        commands = new xf::Vector<Command*>();
        addCommand(new TestCommand()); 
    }

    void addCommand(Command* command) {
        commands->push_back(command);
    }

    void openKeyboardMenu() {
        mc::CInput::GetInput()->RequestKeyboard(L"", L"", 0, 255, [](void* p, bool) -> int {
            Commands* _this = (Commands*) p;
            wchar_t temp[256];
            mc::CInput::GetInput()->GetText(temp, 255);
            xf::String<wchar_t> tempStr = temp;

            if (tempStr.length() == 0) return 0;

            xf::Vector<xf::String<wchar_t>> splitStr;
            if (tempStr.find(L' ') > 0) {
                splitStr = tempStr.split(L' ');
            } else {
                splitStr.push_back(tempStr.c_str());
            }

            for (Command* cmd : *_this->getCommands()) {
                if (cmd->getName().toLowerCase().startsWith(splitStr[0].toLowerCase()) && cmd->getName().length() == splitStr[0].length()) {
                    splitStr.pop_front();
                    cmd->onCall(splitStr);
                    break;
                }
            }

            return 0;
        }, this, mc::CInput::KeyboardMode::FULL);
    }

    xf::Vector<Command*>* getCommands() {
        return commands;
    }

private:
    xf::Vector<Command*>* commands;
};