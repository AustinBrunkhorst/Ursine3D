<<<<<<< HEAD
#include "../dep/Benchmarks/include/hayai.hpp"

#include "Vec2.h"
#include "SVec3.h"
#include "SVec4.h"
=======
#include "../../../Benchmarking/hayai.hpp"

#include "../NewMath/Vec2.h"
#include "../NewMath/Vec3.h"
#include "../NewMath/Vec4.h"
>>>>>>> master

using namespace Ursine;

BENCHMARK(Vec4, DotProduct, 10, 100)
{
<<<<<<< HEAD
    SVec4 vec_0(2.0f);
    SVec4 vec_1(1.5f);
=======
    Vec4 vec_0(2.0f);
    Vec4 vec_1(1.5f);
>>>>>>> master

    auto result = vec_0 - vec_1;
}