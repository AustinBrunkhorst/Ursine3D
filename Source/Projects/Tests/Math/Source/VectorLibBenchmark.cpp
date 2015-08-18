#include "../dep/Benchmarks/include/hayai.hpp"

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

using namespace Ursine;

BENCHMARK(Vec4, DotProduct, 10, 100)
{
    Vec4 vec_0(2.0f);
    Vec4 vec_1(1.5f);

    auto result = vec_0 - vec_1;
}