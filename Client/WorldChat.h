#ifndef WORLDCHAT_H
#define WORLDCHAT_H

#include "Module.h"

#include <minecraft/mc.h>

struct WorldChat* staticWorldChat;

class WorldChat {
public: 
    class FormatSlider : public Slider<int> {
    public:
        FormatSlider() : Slider<int>(0, 4, 0, 1) {
            setDrawFunc([](Slider<int>* _this) -> mstd::wstring {
                wchar_t temp[0x20];
                mc_swprintf(temp, 0x20, staticWorldChat->getFormats()[_this->getCurrent()].c_str(), L"n", L"m");
                return temp;
            });

            setEvents([](Slider<int>* _this) { // Left
                    if (_this->getCurrent() == _this->getMin()) {
                        _this->setCurrent(_this->getMax());
                    } else {
                        _this->setCurrent(_this->getCurrent() - _this->getIncrement());
                    }
                }, [](Slider<int>* _this) { // Right
                    if (_this->getCurrent() == _this->getMax()) {
                        _this->setCurrent(_this->getMin());
                    } else {
                        _this->setCurrent(_this->getCurrent() + _this->getIncrement());
                    }
                }
            );
        }
    }; 
    
    static const int maxLength = 77;

    WorldChat() {
        _module = new Module(L"World Chat", Module::Type::MODULE);
        staticWorldChat = this;

        formats = new mstd::wstring[5];
        formats[0] = L"%ls: %ls";
        formats[1] = L"<%ls> %ls";
        formats[2] = L"<%ls>: %ls";
        formats[3] = L"[%ls] %ls";
        formats[4] = L"[%ls]: %ls";
        
        lastMessage = new wchar_t[maxLength + 1];
        memset(lastMessage, 0x0, sizeof(wchar_t) * maxLength);

        _module->addModuleToSettings(new Module(L"Store Last Message", Module::Type::MODULE));

        Module* sliderMod = new Module(L"Format", Module::Type::SLIDER);
        sliderMod->setSlider(new FormatSlider());
        _module->addModuleToSettings(sliderMod);

         _module->addModuleToSettings(new Module(L"Talk in Chat", Module::Type::BUTTON));
        _module->getPageVector()[2]->setEvents(nullptr, nullptr, [](Module* mod) {
            mc::CInput::GetInput()->RequestKeyboard(L"", staticWorldChat->keepLastMessage() ? staticWorldChat->getLastMessage() : L"", 0, maxLength, [](void* data, bool b) {
                wchar_t temp[maxLength + 1];
                mc::CInput::GetInput()->GetText(temp, maxLength);
                memcpy(staticWorldChat->getLastMessage(), temp, sizeof(wchar_t) * (maxLength + 1));
                if (!staticWorldChat->keepLastMessage()) memset(staticWorldChat->getLastMessage(), 0, sizeof(wchar_t) * (maxLength + 1));

                mc_boost::shared_ptr<mc::Packet> packet(new mc::ClientboundChatPacket(temp));
                mc::Minecraft::getInstance()->getConnection(0)->send(packet);
                return 0;
            }, mod, 0);
        });
    }

    static void handleChat(mc::ServerGamePacketListenerImpl* listener, const mc_boost::shared_ptr<mc::ClientboundChatPacket>& packet) {
        if (packet->str_v[0].length == 0) return; // No Empty Message
        if (!staticWorldChat->getModule()->getState()) return;
        mstd::wstring msg = packet->str_v[0];
        memset((void*) &msg.c_str()[msg.length], 0x0, sizeof(wchar_t)); // I don't know why

        bool isOnlyWhiteSpace = true;
        for (int i = 0; i < msg.length; i++) {
            wchar_t _char = msg.c_str()[i];
            if (_char != L' ') {
                isOnlyWhiteSpace = false;
                break;
            }
        }

        if (isOnlyWhiteSpace) return; // No Empty Message

        mc::ServerPlayer* sender = nullptr;
        for (mc_boost::shared_ptr<mc::ServerPlayer>& player : mc::MinecraftServer::getInstance()->getPlayers()->players) {
            if (player->listener == listener) sender = player.get();
        }

        if (sender) {
            wchar_t temp[150];
            mstd::wstring playerName;
            sender->getDisplayName(playerName);
            mc_swprintf(temp, 150, staticWorldChat->getFormats()[staticWorldChat->formatIndex()].c_str(), playerName.c_str(), msg.c_str());
            if (mstd::wstring(temp).length > 95) return;
            mc::MinecraftServer::getInstance()->getPlayers()->broadcastAll(new mc::ClientboundChatPacket(temp));
            mc::MinecraftServer::getInstance()->getPlayers()->broadcastAll(new mc::ClientboundSoundPacket(mc::SoundEvent::note_hat, 1.0f, 1.0f));
        } else {
            mc_printf(L"Error %s %d", __FILE__, __LINE__);
        }
    }

    bool keepLastMessage() {
        return getModule()->getPageVector()[0]->getState();
    }

    int formatIndex() {
        return ((FormatSlider*) _module->getPageVector()[1]->getSlider())->getCurrent();
    }

    mstd::wstring* getFormats() {
        return formats;
    }

    wchar_t* getLastMessage() {
        return lastMessage;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
    wchar_t* lastMessage;
    mstd::wstring* formats;
};

#endif