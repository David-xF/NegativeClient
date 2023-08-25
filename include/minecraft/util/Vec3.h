#pragma once

#include <cmath>

#include "FloatConverter.h"

namespace mc {
    class Vec3 {
    public:
        Vec3(int _x, int _y, int _z) {
            x = toFloat(_x) + 0.5f;
            y = toFloat(_y);
            z = toFloat(_z) + 0.5f;
        }

        Vec3() = default;

        float distance(Vec3 other) {
            return sqrtf(
                powf(x - other.x, 2) + 
                powf(y - other.y, 2) + 
                powf(z - other.z, 2)
            );
        }

        double x;
        double y;
        double z;
    };
}