#pragma once

#include <cstdint>
#include <random>
#include <ctime>

namespace Random
{
    void init();
    int64_t intInRange(int64_t lBound, int64_t hBound);
}
