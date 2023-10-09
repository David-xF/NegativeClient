#ifndef FOFBYPASS_H
#define FOFBYPASS_H

#include <code/code.h>

#include "Module.h"

#include <minecraft/mc.h>

struct FriendsOfFriendsBypass* staticFofBypass;

class FriendsOfFriendsBypass {
public:
    FriendsOfFriendsBypass() {
        _module = new Module(L"Bypass Friends Only", Module::Type::MODULE);
        staticFofBypass = this;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif