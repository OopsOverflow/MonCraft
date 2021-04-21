#include "Random.hpp"

namespace Random
{
    namespace
    {
        std::mt19937 rd;
    }

    void init()
    {
        rd.seed(std::time(nullptr));
    }

    int64_t intInRange(int64_t lBound, int64_t hBound)
    {
        std::uniform_int_distribution<int64_t> dist(lBound, hBound);
        return dist(rd);
    }
}