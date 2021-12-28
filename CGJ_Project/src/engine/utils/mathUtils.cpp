#include "mathUtils.h"




float invSqrt(float number) {
    union {
        float f;
        uint32_t i;
    } conv;

    float x2 = number * 0.5F;
    const float threehalfs = 1.5F;

    conv.f = number;
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
    return conv.f;
}