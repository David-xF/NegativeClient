#ifndef HEALTHINDICATOR_H
#define HEALTHINDICATOR_H

#include "Module.h"

#include <minecraft/mc.h>

void* staticHealthIndicator;

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

        Module* sliderMod = new Module(L"Mode", Module::Type::SLIDER);
        sliderMod->setSlider(new HealthSlider());
        _module->addModuleToSettings(sliderMod);
    }

    static int getMode() {
        HealthIndicator* healthIndicator = (HealthIndicator*) staticHealthIndicator;
        Slider<int>* slider = (Slider<int>*) healthIndicator->getModule()->getPageVector()[0]->getSlider();
        return slider->getCurrent();
    }

    static const wchar_t* getHP(mc::Player* player) {
        wchar_t* temp = new wchar_t[0x40];
        mc_swprintf(temp, 0x40, L"%s", toCStr(player->getHealth(), 2));
        return temp;
    }

    static mc::Player* getPlayer(const mstd::wstring& name) {
        HealthIndicator* healthIndicator = (HealthIndicator*) staticHealthIndicator;
        if (!healthIndicator->getModule()->getState()) return nullptr;

        mc_boost::shared_ptr<mc::Player> player;
        mc::Minecraft::getInstance()->getLevel(0)->getPlayerByName(player, name);

        return player.ptr;
    }

    static const wchar_t* displayName(const mstd::wstring& name) {
        HealthIndicator* healthIndicator = (HealthIndicator*) staticHealthIndicator;
        if (!healthIndicator->getModule()->getState()) return name.c_str();

        mc_boost::shared_ptr<mc::Player> player;
        mc::Minecraft::getInstance()->getLevel(0)->getPlayerByName(player, name);

        if (!player.ptr) return name.c_str();
    
        wchar_t* temp = new wchar_t[0x40];
        mc_swprintf(temp, 0x40, L"%ls [%ls]", name.c_str(), getHP(player.ptr));
        
        return temp;
    }

    Module* getModule() {
        return _module;
    }

private:
    Module* _module;
};

#endif