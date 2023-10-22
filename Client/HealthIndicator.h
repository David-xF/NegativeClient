#ifndef HEALTHINDICATOR_H
#define HEALTHINDICATOR_H

#include "Module.h"

#include <minecraft/mc.h>

struct HealthIndicator* staticHealthIndicator;

class HealthIndicator {
public:
    class HealthSlider : public Slider<int> {
    public:
        HealthSlider() : Slider<int>(0, 2, 0, 1) {
            setDrawFunc([](Slider<int>* _this) -> mstd::wstring {
                mstd::wstring labels[] = {
                    L"Inline", L"Above", L"Below"
                };
                return labels[_this->getCurrent()];
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

    HealthIndicator() {
        _module = new Module(L"Health Indicator", Module::Type::MODULE);
        staticHealthIndicator = this;

        _module->addModuleToSettings(new Module(L"Health Color", Module::Type::MODULE));
        _module->addModuleToSettings(new Module(L"As Percentage", Module::Type::MODULE));

        Module* sliderMod = new Module(L"Mode", Module::Type::SLIDER);
        sliderMod->setSlider(new HealthSlider());
        _module->addModuleToSettings(sliderMod);
    }

    static bool shouldColorBasedOnHealth() {
        return staticHealthIndicator->getModule()->getPageVector()[0]->getState();
    }

    static bool shouldShowInPercent() {
        return staticHealthIndicator->getModule()->getPageVector()[1]->getState();
    }

    static int getMode() {
        Slider<int>* slider = (Slider<int>*) staticHealthIndicator->getModule()->getPageVector()[2]->getSlider();
        return slider->getCurrent();
    }

    static xf::String<wchar_t> getHP(mc::Player* player) {
        const char* colors[] = {
            "6", // INF% - 100%
            "a", // 100% - 50% 
            "e", // 50%  - 25% 
            "c"  // 25%  - 0%
        };

        char* col = (char*) "f";
        float mHealth = player->getMaxHealth();
        float health = player->getHealth() + player->getAbsorptionAmount();
        if (shouldColorBasedOnHealth()) {
            if ((mHealth / 4.0f) >= health) {
                col = (char*) colors[3];
            } else if ((mHealth / 2.0f) >= health) {
                col = (char*) colors[2];
            } else if (mHealth >= health) {
                col = (char*) colors[1];
            } else {
                if (player->getAbsorptionAmount() > 0) {
                    col = (char*) colors[0];
                } else {
                    col = (char*) colors[1];
                }
            }
        }

        wchar_t temp[0x40];
        if (shouldShowInPercent()) {
            mc_swprintf(temp, 0x40, L"§%s%s%s§f", col, toCStr((health / mHealth) * 100, 2), "%");
        } else {
            mc_swprintf(temp, 0x40, L"§%s%s§f", col, toCStr(health, 2));
        }

        return temp;
    }

    static mc::Player* getPlayer(const mstd::wstring& name) {
        if (!staticHealthIndicator->getModule()->getState()) return nullptr;

        mc_boost::shared_ptr<mc::Player> player;
        mc::Minecraft::getInstance()->getLevel(0)->getPlayerByName(player, name);

        return player.get();
    }

    static xf::String<wchar_t> displayName(const mstd::wstring& name) {
        if (!staticHealthIndicator->getModule()->getState()) return name.c_str();

        mc::Player* player = getPlayer(name);
        if (!player) return name.c_str();
    
        wchar_t temp[0x40];
        mc_swprintf(temp, 0x40, L"§f%ls [%ls]", name.c_str(), getHP(player).c_str());
    
        return temp;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif