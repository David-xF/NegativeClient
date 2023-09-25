#ifndef SLIDER_H
#define SLIDER_H

#include <minecraft/mc.h>

template<typename T = float>
class Slider {
public:
    Slider(T minVal, T maxVal, T defVal, T incr) {
        minimum = minVal;
        maximum = maxVal;
        current = defVal;
        increment = incr;

        setDrawFunc(onDefaultDrawFunc);
        setEvents(onDefaultEvent, onDefaultEvent);
    }

    static mstd::wstring onDefaultDrawFunc(Slider<T>* _this) {
        return L"Not Implemented";
    }

    static void onDefaultEvent(Slider<T>* _this) {
        mc_printf(L"Default");
        return;
    }

    void setEvents(void(*_onLeft)(Slider<T>*), void(*_onRight)(Slider<T>*)) {
        onLeft = _onLeft;
        onRight = _onRight;
    }

    void setDrawFunc(mstd::wstring(*_onDraw)(Slider<T>*)) {
        onDraw = _onDraw;
    }

    // mstd::wstring(*onDraw)(Slider<T>*);
    void* getDrawFunc() {
        return (void*) onDraw;
    }

    void* getEvent(int which) {
        if (which == 0) return (void*) onLeft;
        else            return (void*) onRight;
    }

    T getMin() {
        return minimum;
    }

    T getMax() {
        return maximum;
    }

    T getCurrent() {
        return current;
    }

    T getIncrement() {
        return increment;
    }

    void setCurrent(T t) {
        current = t;
    }

private:
    mstd::wstring(*onDraw)(Slider<T>*);
    void(*onLeft)(Slider<T>*);
    void(*onRight)(Slider<T>*);
    T minimum;
    T maximum;
    T current;
    T increment;
};

#endif