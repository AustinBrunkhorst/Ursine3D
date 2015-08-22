#include "../dep/Benchmarks/include/hayai.hpp"

#include "Vec2.h"
#include "SVec3.h"
#include "SVec4.h"

using namespace Ursine;

BENCHMARK(Vec4, DotProduct, 10, 100)
{
    SVec4 vec_0(2.0f);
    SVec4 vec_1(1.5f);

    auto result = vec_0 - vec_1;
}