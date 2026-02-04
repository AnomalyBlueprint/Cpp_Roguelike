#pragma once
#include <algorithm> // For std::max, std::min

namespace Math
{

    inline float Lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    inline float Clamp(float value, float min, float max)
    {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }
}