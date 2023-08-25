#ifndef MODULE_H
#define MODULE_H

#include <minecraft/mc.h>

#include <memory>

#include <xf/Vector.h>

class Module {
public:
    enum Type {
        PAGE, MODULE, SLIDER, BUTTON
    };

    Module(const wchar_t* name, Type type) {
        const int maxNameSize = 0x20;
        moduleName = new wchar_t[maxNameSize];
        memcpy(moduleName, name, maxNameSize);
        moduleType = type;
        pageVector = xf::Vector<Module*>();
        isEnabled = false;
        settings = nullptr;

        onEnable = nullptr;
        onDisable = nullptr;
        onClick = nullptr;

        selectIndex = 0;
        pageIndex = -1;
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

    void toggleState() {
        setState(!isEnabled);
    }

    void setState(bool state) {
        if (moduleType == Type::BUTTON) {
            if (onClick != nullptr) onClick(this);
            return;
        }
        isEnabled = state;
        if (onEnable == nullptr || onDisable == nullptr) return;
        isEnabled ? onEnable(this) : onDisable(this); 
    }

    bool getState() {
        return isEnabled;
    }

    void setEvents(void(*onEn)(Module*), void(*onDis)(Module*), void(*onCl)(Module*)) {
        onEnable  = (void(*)(Module*)) mc::PointerFix::Fix(onEn);
        onDisable = (void(*)(Module*)) mc::PointerFix::Fix(onDis);
        onClick   = (void(*)(Module*)) mc::PointerFix::Fix(onCl);
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
            0x55FF55  // Enabled
        };

        if (moduleType == Type::PAGE) return colors[0];
        else if (moduleType == Type::BUTTON) return colors[1];

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

    void right() {
        if (pageIndex == -1) {
            if (pageVector[selectIndex]->getType() == Type::PAGE) {
                pageIndex = selectIndex;
                pageVector[pageIndex]->reset();
            } else {
                pageVector[selectIndex]->toggleState();
            }
        } else {
            pageVector[pageIndex]->right();
        }
    }

    bool left() {
        if (pageIndex != -1) {
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

private:
    xf::Vector<Module*> pageVector;
    Module* settings;
    wchar_t* moduleName;
    Type moduleType;
    bool isEnabled;

    void(*onEnable)(Module*);
    void(*onDisable)(Module*);
    void(*onClick)(Module*);

    int selectIndex;
    int pageIndex;
};

#endif