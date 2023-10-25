#ifndef MODULE_H
#define MODULE_H

#include <minecraft/mc.h>

#include <memory>

#include <xf/Vector.h>

#include "Slider.h"

class Module {
public:
    enum Type {
        PAGE, MODULE, SLIDER, BUTTON
    };

    Module(const wchar_t* name, Type type) {
        moduleName = new wchar_t[mc_wcslen(name) + 1];
        mc_wcscpy(moduleName, name);
        moduleType = type;
        pageVector = xf::Vector<Module*>();
        isEnabled = false;

        onEnable = nullptr;
        onDisable = nullptr;
        onClick = nullptr;

        selectIndex = 0;
        pageIndex = -1;

        setSlider(nullptr);
    }

    void addModuleToVector(Module* page) {
        if (moduleType != Type::PAGE) return;
        pageVector.push_back(page);
    }

    const wchar_t* getName() {
        return moduleName;
    }

    Type getType() {
        return moduleType;
    }

    xf::Vector<Module*> getPageVector() {
        return pageVector;
    }

    Module* toggleState() {
        setState(!isEnabled);
        return this;
    }

    void setState(bool state) {
        if (moduleType == Type::BUTTON) {
            if (onClick) onClick(this);
            return;
        }
        isEnabled = state;
        if (!onEnable || !onDisable) return;
        isEnabled ? onEnable(this) : onDisable(this); 
    }

    void addModuleToSettings(Module* _settings) {
        pageVector.push_back(_settings);
    }

    bool getState() {
        return isEnabled;
    }

    void setEvents(void(*onEn)(Module*), void(*onDis)(Module*), void(*onCl)(Module*)) {
        onEnable  = (void(*)(Module*)) onEn;
        onDisable = (void(*)(Module*)) onDis;
        onClick   = (void(*)(Module*)) onCl;
    }

    Module* at(int index) {
        return pageVector[index];
    }

    int selctedInt() {
        return selectIndex;
    }

    int getPageIndex() {
        return pageIndex;
    }

    uint32_t getColor() {
        // 0x99CCFF = Light Sky Blue
        const uint32_t colors[] = {
            0x99CCFF, // Page
            0xFFAA00, // Button
            0xFF5555, // Disabled 
            0x55FF55, // Enabled
            0xFFFFFF  // Slider
        };

        if (moduleType == Type::PAGE) return colors[0];
        else if (moduleType == Type::BUTTON) return colors[1];
        else if (moduleType == Type::SLIDER) return colors[4];

        return getState() ? colors[3] : colors[2];
    }

    void up() {
        if (pageIndex == -1) {
            selectIndex--;
            if (selectIndex == -1) selectIndex = pageVector.getSize() - 1;
        } else {
            pageVector[pageIndex]->up();
        }
    }

    void down() {
        if (pageIndex == -1) {
            selectIndex++;
            if (selectIndex == pageVector.getSize()) selectIndex = 0;
        } else {
            pageVector[pageIndex]->down();
        }
    }

    void reset() {
        selectIndex = 0;
        pageIndex = -1;
    }

    bool hasSettings() {
        if (getType() == Type::MODULE || getType() == Type::BUTTON) {
            if (pageVector.getSize() > 0) return true;
            return false;
        } else {
            return false;
        }
    }

    void openSettings() {
        if (pageVector[selectIndex]->getPageVector().getSize() > 0) {
            pageIndex = selectIndex;
            pageVector[pageIndex]->reset();
        }
    }

    void right() {
        if (pageIndex == -1) {
            if (pageVector[selectIndex]->getType() == Type::PAGE) {
                if (pageVector[selectIndex]->getPageVector().getSize() > 0) {
                    pageIndex = selectIndex;
                    pageVector[pageIndex]->reset();
                }
            } else {
                if (pageVector[selectIndex]->isSlider()) {
                    Slider<>* _slider = (Slider<>*) pageVector[selectIndex]->getSlider();
                    ((void(*)(Slider<>*)) _slider->getEvent(1))(_slider);
                } else pageVector[selectIndex]->toggleState();
            }
        } else {
            pageVector[pageIndex]->right();
        }
    }

    bool left() {
        if (pageIndex == -1) {
            if (pageVector[selectIndex]->isSlider()) {
                Slider<>* _slider = (Slider<>*) pageVector[selectIndex]->getSlider();
                ((void(*)(Slider<>*)) _slider->getEvent(0))(_slider);
                return true;
            }
        } else if (pageIndex != -1) {
            if (pageVector[pageIndex]->left()) return true;
            if (pageVector[pageIndex]->getPageIndex() == -1) {
                pageIndex = -1;
                return true;
            }
        }

        return false;
    }

    static bool isBlackListed(uint64_t type) {
        uint64_t blackList[] = {
            mc::ItemEntity::GetType(),
            mc::AreaEffectCloud::GetType()
        };

        for (uint64_t t : blackList) {
            if (t == type) return true;
        }

        return (!(type & mc::LivingEntity::GetType()));
    }

    void setSlider(void* _slider) {
        slider = (Slider<>*) _slider;
    }

    void* getSlider() {
        return (void*) slider;
    }

    bool isSlider() {
        return getType() == Type::SLIDER;
    }

private:
    // Module
    wchar_t* moduleName;
    Type moduleType;
    bool isEnabled;

    // Events
    void(*onEnable)(Module*);
    void(*onDisable)(Module*);
    void(*onClick)(Module*); // Button Only

    // Page / Settings
    int selectIndex;
    int pageIndex;
    xf::Vector<Module*> pageVector;

    // Slider
    Slider<>* slider;
};

#endif