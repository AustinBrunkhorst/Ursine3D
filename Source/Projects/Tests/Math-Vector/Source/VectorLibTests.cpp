/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VectorLibTests.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#define CATCH_CONFIG_MAIN
#include "../dep/Testing/include/catch.hpp"

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "SVec3.h"
#include "SVec4.h"

using namespace ursine;

/////////////////////////////////////////////////////////
// Construction Tests
/////////////////////////////////////////////////////////
TEST_CASE("Vec2: Default Constructor")
{
    Vec2 vec;
    REQUIRE(vec.X() == 0.0f);
    REQUIRE(vec.Y() == 0.0f);
}

TEST_CASE("Vec2: Copy Constructor")
{
    Vec2 foo(1.0f, 2.0f);
    Vec2 bar(foo);

    REQUIRE(foo.X() == bar.X());
    REQUIRE(foo.Y() == bar.Y());
}

TEST_CASE("Vec2: Explicit Float Constructor")
{
    Vec2 vec(4.0f);

    REQUIRE(vec.X() == 4.0f);
    REQUIRE(vec.Y() == 4.0f);
}

TEST_CASE("Vec2: Explicit SVec3 Constructor")
{
    SVec3 foo(1.0f);
    Vec2 bar(foo);

    REQUIRE(bar.X() == foo.X());
    REQUIRE(bar.Y() == foo.Y());
}

TEST_CASE("SVec3: Default Constructor")
{
    SVec3 vec;
    
    REQUIRE(vec.X() == 0.0f);
    REQUIRE(vec.Y() == 0.0f);
    REQUIRE(vec.Z() == 0.0f);
}

TEST_CASE("SVec3: Copy Constructor")
{
    SVec3 foo(1.0f, 2.0f, 3.0f);
    SVec3 bar(foo);

    REQUIRE(foo.X() == bar.X());
    REQUIRE(foo.Y() == bar.Y());
    REQUIRE(foo.Z() == bar.Z());
}

TEST_CASE("SVec3: Explicit Float Constructor")
{
    SVec3 vec(4.0f);

    REQUIRE(vec.X() == 4.0f);
    REQUIRE(vec.Y() == 4.0f);
    REQUIRE(vec.Z() == 4.0f);
}

TEST_CASE("SVec3: Vec2 with Z Constructor")
{
    Vec2 foo(1.0f, 2.0f);
    SVec3 bar(foo, 3.0f);

    REQUIRE(bar.X() == 1.0f);
    REQUIRE(bar.Y() == 2.0f);
    REQUIRE(bar.Z() == 3.0f);
}

TEST_CASE("SVec3: SVec4 Explicit Constructor")
{
    SVec4 foo(1.0f, 2.0f, 3.0f, 4.0f);

    SVec3 bar(foo);

    REQUIRE(bar.X() == 1.0f);
    REQUIRE(bar.Y() == 2.0f);
    REQUIRE(bar.Z() == 3.0f);
}

TEST_CASE("SVec4: Default Constructor")
{
    SVec4 vec;
    
    REQUIRE(vec.X() == 0.0f);
    REQUIRE(vec.Y() == 0.0f);
    REQUIRE(vec.Z() == 0.0f);
    REQUIRE(vec.W() == 0.0f);
}

TEST_CASE("SVec4: Copy Constructor")
{
    SVec4 foo(1.0f, 2.0f, 3.0f, 4.0f);
    SVec4 bar(foo);

    REQUIRE(foo.X() == bar.X());
    REQUIRE(foo.Y() == bar.Y());
    REQUIRE(foo.Z() == bar.Z());
    REQUIRE(foo.W() == bar.W());
}

TEST_CASE("SVec4: Explicit Float Constructor")
{
    SVec4 vec(4.0f);

    REQUIRE(vec.X() == 4.0f);
    REQUIRE(vec.Y() == 4.0f);
    REQUIRE(vec.Z() == 4.0f);
    REQUIRE(vec.W() == 4.0f);
}

TEST_CASE("SVec4: SVec3 with W Constructor")
{
    SVec3 foo(1.0f, 2.0f, 3.0f);
    SVec4 bar(foo, 4.0f);

    REQUIRE(bar.X() == 1.0f);
    REQUIRE(bar.Y() == 2.0f);
    REQUIRE(bar.Z() == 3.0f);
    REQUIRE(bar.W() == 4.0f);
}
/////////////////////////////////////////////////////////
// !Construction Tests
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// Accessor Tests
/////////////////////////////////////////////////////////

TEST_CASE("Vec2: XY Const Getter")
{
    const Vec2 vec(1.0f, 2.0f);

    // This should break
    // vec.X() = 4;
    // vec.Y() = 2;

    REQUIRE(vec.X() == 1.0f);
    REQUIRE(vec.Y() == 2.0f);
}

TEST_CASE("Vec2: XY Mutable Getter")
{
    Vec2 vec(1.0f, 2.0f);

    vec.X() = 2.0f;
    vec.Y() = 1.0f;

    REQUIRE(vec.X() == 2.0f);
    REQUIRE(vec.Y() == 1.0f);
}

TEST_CASE("Vec2: XY Const Index Operator")
{
    const Vec2 vec(1.0f, 2.0f);

    // This should break
    // vec[0] = 2.0f;
    // vec[1] = 3.0f;

    REQUIRE(vec.X() == 1.0f);
    REQUIRE(vec.Y() == 2.0f);
}

TEST_CASE("Vec2: XY Index Operator")
{
    Vec2 vec(1.0f, 2.0f);

    vec[0] = 2.0f;
    vec[1] = 1.0f;

    REQUIRE(vec.X() == 2.0f);
    REQUIRE(vec.Y() == 1.0f);
}

TEST_CASE("SVec3: XYZ Const Getter")
{
    const SVec3 vec(1.0f, 2.0f, 3.0f);

    // This should break
    // vec.X() = 4;
    // vec.Y() = 2;
    // vec.Z() = 1;

    REQUIRE(vec.X() == 1.0f);
    REQUIRE(vec.Y() == 2.0f);
    REQUIRE(vec.Z() == 3.0f);
}

TEST_CASE("SVec3: XYZ Mutable Getter")
{
    SVec3 vec(1.0f, 2.0f, 3.0f);

    vec.X() = 3.0f;
    vec.Y() = 2.0f;
    vec.Z() = 1.0f;

    REQUIRE(vec.X() == 3.0f);
    REQUIRE(vec.Y() == 2.0f);
    REQUIRE(vec.Z() == 1.0f);
}

TEST_CASE("SVec3: XYZ Const Index Operator")
{
    const SVec3 vec(1.0f, 2.0f, 3.0f);

    // This should break
    // vec[0] = 2.0f;
    // vec[1] = 3.0f;
    // vec[2] = 3.0f;

    REQUIRE(vec.X() == 1.0f);
    REQUIRE(vec.Y() == 2.0f);
    REQUIRE(vec.Z() == 3.0f);
}

TEST_CASE("SVec3: XYZ Index Operator")
{
    SVec3 vec(1.0f, 2.0f, 3.0f);

    vec[0] = 3.0f;
    vec[1] = 2.0f;
    vec[2] = 1.0f;

    REQUIRE(vec.X() == 3.0f);
    REQUIRE(vec.Y() == 2.0f);
    REQUIRE(vec.Z() == 1.0f);
}

TEST_CASE("SVec4: XYZW Const Getter")
{
    const SVec4 vec(1.0f, 2.0f, 3.0f, 4.0f);

    // This should break
    // vec.X() = 4;
    // vec.Y() = 2;
    // vec.Z() = 1;
    // vec.W() = 1;

    REQUIRE(vec.X() == 1.0f);
    REQUIRE(vec.Y() == 2.0f);
    REQUIRE(vec.Z() == 3.0f);
    REQUIRE(vec.W() == 4.0f);
}

TEST_CASE("SVec4: XYZW Mutable Getter")
{
    SVec4 vec(1.0f, 2.0f, 3.0f, 4.0f);

    vec.X() = 4.0f;
    vec.Y() = 3.0f;
    vec.Z() = 2.0f;
    vec.W() = 1.0f;

    REQUIRE(vec.X() == 4.0f);
    REQUIRE(vec.Y() == 3.0f);
    REQUIRE(vec.Z() == 2.0f);
    REQUIRE(vec.W() == 1.0f);
}

TEST_CASE("SVec4: XYZW Const Index Operator")
{
    const SVec4 vec(1.0f, 2.0f, 3.0f, 4.0f);

    // This should break
    // vec[0] = 2.0f;
    // vec[1] = 3.0f;
    // vec[2] = 3.0f;
    // vec[3] = 5;

    REQUIRE(vec.X() == 1.0f);
    REQUIRE(vec.Y() == 2.0f);
    REQUIRE(vec.Z() == 3.0f);
    REQUIRE(vec.W() == 4.0f);
}

TEST_CASE("SVec4: XYZW Index Operator")
{
    SVec4 vec(1.0f, 2.0f, 3.0f, 4.0f);

    vec[0] = 4.0f;
    vec[1] = 3.0f;
    vec[2] = 2.0f;
    vec[3] = 1.0f;

    REQUIRE(vec.X() == 4.0f);
    REQUIRE(vec.Y() == 3.0f);
    REQUIRE(vec.Z() == 2.0f);
    REQUIRE(vec.W() == 1.0f);
}

/////////////////////////////////////////////////////////
// !Accessor Tests
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// Operator Tests
/////////////////////////////////////////////////////////

TEST_CASE("Vec2: Comparison Operators")
{
    Vec2 foo(1.0f);
    Vec2 bar(1.0f);
    Vec2 foobar(2.0f);

    REQUIRE(foo == bar);
    REQUIRE(foo != foobar);
}

TEST_CASE("Vec2: Addition Operator")
{
    const Vec2 foo(1.0f);
    const Vec2 bar(1.0f);
    Vec2 foobar = foo + bar;

    REQUIRE(foobar.X() == 2.0f);
    REQUIRE(foobar.Y() == 2.0f);
}

TEST_CASE("Vec2: Negation Operator")
{
    const Vec2 foo(2.0f);
    const Vec2 bar = -foo;

    REQUIRE(bar.X() == -2.0f);
    REQUIRE(bar.Y() == -2.0f);
}

TEST_CASE("Vec2: Subtraction Operator")
{
    const Vec2 foo(2.0f);
    const Vec2 bar(1.0f);
    Vec2 foobar = foo - bar;

    REQUIRE(foobar == bar);
}

TEST_CASE("Vec2: Multiplication Operator")
{
    const Vec2 foo(2.0f);
    const Vec2 bar(0.5f);
    Vec2 foobar = foo * bar;

    REQUIRE(foobar == Vec2::One());
}

TEST_CASE("Vec2: Scalar Operator")
{
    const Vec2 foo(1.0f);
    const Vec2 bar = foo * 40.0f;
    const Vec2 foobar = 20.0f * bar;

    REQUIRE(bar == Vec2(40.0f));
    REQUIRE(foobar == Vec2(800.0f));
}

TEST_CASE("Vec2: Division Operator")
{
    const Vec2 foo(4.0f);
    const Vec2 bar = foo / 2.0f;
    const Vec2 foobar = foo / bar;

    REQUIRE(bar == Vec2(2.0f));
    REQUIRE(foobar == Vec2(2.0f));
}

TEST_CASE("Vec2: Assignment Operator")
{
    const Vec2 foo(1.0f);
    Vec2 bar;

    bar = foo;

    REQUIRE(bar == Vec2(1.0f));
}

TEST_CASE("Vec2: Plus Equal Operator")
{
    Vec2 foo(1.0f);
    Vec2 bar(1.0f);

    bar += foo;

    REQUIRE(bar == Vec2(2.0f));
}

TEST_CASE("Vec2: Minus Equal Operator")
{
    Vec2 foo(1.0f);
    Vec2 bar(1.0f);

    bar -= foo;

    REQUIRE(bar == Vec2());
}

TEST_CASE("Vec2: Times Equal Operator")
{
    Vec2 foo(1.25f);
    Vec2 bar(1.0f);

    bar *= foo;

    REQUIRE(bar == Vec2(1.25f));
}

TEST_CASE("Vec2: Times Equal Scalar Operator")
{
    Vec2 foo(1.0f);
    float val = 5.0f;

    foo *= val;

    REQUIRE(foo == Vec2(5.0f));
}

TEST_CASE("Vec2: Division Equal Operators")
{
    Vec2 foo(4.0f);
    Vec2 bar = foo / 2.0f;
    Vec2 foobar = foo / bar;

    REQUIRE(bar == Vec2(2.0f));
    REQUIRE(foobar == Vec2(2.0f));
}

TEST_CASE("SVec3: Comparison Operators")
{
    SVec3 foo(1.0f);
    SVec3 bar(1.0f);
    SVec3 foobar(2.0f);

    REQUIRE(foo == bar);
    REQUIRE(foo != foobar);
}

TEST_CASE("SVec3: Addition Operator")
{
    const SVec3 foo(1.0f);
    const SVec3 bar(1.0f);
    SVec3 foobar = foo + bar;

    REQUIRE(foobar.X() == 2.0f);
    REQUIRE(foobar.Y() == 2.0f);
    REQUIRE(foobar.Z() == 2.0f);
}

TEST_CASE("SVec3: Negation Operator")
{
    const SVec3 foo(2.0f);
    const SVec3 bar = -foo;

    REQUIRE(bar.X() == -2.0f);
    REQUIRE(bar.Y() == -2.0f);
    REQUIRE(bar.Z() == -2.0f);
}

TEST_CASE("SVec3: Subtraction Operator")
{
    const SVec3 foo(2.0f);
    const SVec3 bar(1.0f);
    SVec3 foobar = foo - bar;

    REQUIRE(foobar == bar);
}

TEST_CASE("SVec3: Multiplication Operator")
{
    const SVec3 foo(2.0f);
    const SVec3 bar(0.5f);
    SVec3 foobar = foo * bar;

    REQUIRE(foobar == SVec3::One());
}

TEST_CASE("SVec3: Scalar Operator")
{
    const SVec3 foo(1.0f);
    const SVec3 bar = foo * 40.0f;
    const SVec3 foobar = 20.0f * bar;

    REQUIRE(bar == SVec3(40.0f));
    REQUIRE(foobar == SVec3(800.0f));
}

TEST_CASE("SVec3: Division Operator")
{
    const SVec3 foo(4.0f);
    const SVec3 bar = foo / 2.0f;
    const SVec3 foobar = foo / bar;

    REQUIRE(bar == SVec3(2.0f));
    REQUIRE(foobar == SVec3(2.0f));
}

TEST_CASE("SVec3: Assignment Operator")
{
    const SVec3 foo(1.0f);
    SVec3 bar;

    bar = foo;

    REQUIRE(bar == SVec3(1.0f));
}

TEST_CASE("SVec3: Plus Equal Operator")
{
    SVec3 foo(1.0f);
    SVec3 bar(1.0f);

    bar += foo;

    REQUIRE(bar == SVec3(2.0f));
}

TEST_CASE("SVec3: Minus Equal Operator")
{
    SVec3 foo(1.0f);
    SVec3 bar(1.0f);

    bar -= foo;

    REQUIRE(bar == SVec3());
}

TEST_CASE("SVec3: Times Equal Operator")
{
    SVec3 foo(1.25f);
    SVec3 bar(1.0f);

    bar *= foo;

    REQUIRE(bar == SVec3(1.25f));
}

TEST_CASE("SVec3: Times Equal Scalar Operator")
{
    SVec3 foo(1.0f);
    float val = 5.0f;

    foo *= val;

    REQUIRE(foo == SVec3(5.0f));
}

TEST_CASE("SVec3: Division Equal Operators")
{
    SVec3 foo(4.0f);
    SVec3 bar = foo / 2.0f;
    SVec3 foobar = foo / bar;

    REQUIRE(bar == SVec3(2.0f));
    REQUIRE(foobar == SVec3(2.0f));
}

TEST_CASE("SVec4: Comparison Operators")
{
    SVec4 foo(1.0f);
    SVec4 bar(1.0f);
    SVec4 foobar(2.0f);

    REQUIRE(foo == bar);
    REQUIRE(foo != foobar);
}

TEST_CASE("SVec4: Addition Operator")
{
    const SVec4 foo(1.0f);
    const SVec4 bar(1.0f);
    SVec4 foobar = foo + bar;

    REQUIRE(foobar.X() == 2.0f);
    REQUIRE(foobar.Y() == 2.0f);
    REQUIRE(foobar.Z() == 2.0f);
    REQUIRE(foobar.W() == 2.0f);
}

TEST_CASE("SVec4: Negation Operator")
{
    const SVec4 foo(2.0f);
    const SVec4 bar = -foo;

    REQUIRE(bar.X() == -2.0f);
    REQUIRE(bar.Y() == -2.0f);
    REQUIRE(bar.Z() == -2.0f);
    REQUIRE(bar.W() == -2.0f);
}

TEST_CASE("SVec4: Subtraction Operator")
{
    const SVec4 foo(2.0f);
    const SVec4 bar(1.0f);
    SVec4 foobar = foo - bar;

    REQUIRE(foobar == bar);
}

TEST_CASE("SVec4: Multiplication Operator")
{
    const SVec4 foo(2.0f);
    const SVec4 bar(0.5f);
    SVec4 foobar = foo * bar;

    REQUIRE(foobar == SVec4::One());
}

TEST_CASE("SVec4: Scalar Operator")
{
    const SVec4 foo(1.0f);
    const SVec4 bar = foo * 40.0f;
    const SVec4 foobar = 20.0f * bar;

    REQUIRE(bar == SVec4(40.0f));
    REQUIRE(foobar == SVec4(800.0f));
}

TEST_CASE("SVec4: Division Operator")
{
    const SVec4 foo(4.0f);
    const SVec4 bar = foo / 2.0f;
    const SVec4 foobar = foo / bar;

    REQUIRE(bar == SVec4(2.0f));
    REQUIRE(foobar == SVec4(2.0f));
}

TEST_CASE("SVec4: Assignment Operator")
{
    const SVec4 foo(1.0f);
    SVec4 bar;

    bar = foo;

    REQUIRE(bar == SVec4(1.0f));
}

TEST_CASE("SVec4: Plus Equal Operator")
{
    SVec4 foo(1.0f);
    SVec4 bar(1.0f);

    bar += foo;

    REQUIRE(bar == SVec4(2.0f));
}

TEST_CASE("SVec4: Minus Equal Operator")
{
    SVec4 foo(1.0f);
    SVec4 bar(1.0f);

    bar -= foo;

    REQUIRE(bar == SVec4());
}

TEST_CASE("SVec4: Times Equal Operator")
{
    SVec4 foo(1.25f);
    SVec4 bar(1.0f);

    bar *= foo;

    REQUIRE(bar == SVec4(1.25f));
}

TEST_CASE("SVec4: Times Equal Scalar Operator")
{
    SVec4 foo(1.0f);
    float val = 5.0f;

    foo *= val;

    REQUIRE(foo == SVec4(5.0f));
}

TEST_CASE("SVec4: Division Equal Operators")
{
    SVec4 foo(4.0f);
    SVec4 bar = foo / 2.0f;
    SVec4 foobar = foo / bar;

    REQUIRE(bar == SVec4(2.0f));
    REQUIRE(foobar == SVec4(2.0f));
}

/////////////////////////////////////////////////////////
// !Operator Tests
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// Public Method Tests
/////////////////////////////////////////////////////////

TEST_CASE("Vec2: Clean Function")
{
    Vec2 clean_me(0.00001f);

    clean_me.Clean();

    REQUIRE(clean_me.X() == 0.0f);
}

TEST_CASE("Vec2: Set Function")
{
    Vec2 set_me;

    set_me.Set(1.0f, 1.0f);

    REQUIRE(set_me == Vec2(1.0f));
}

TEST_CASE("Vec2: Clamp Functions")
{
    const Vec2 result(5.0f, -5.0f);
    Vec2 clamp_me(10.0f, -10.0f);
    Vec2 clamp_me_2(10.0f, -10.0f);
    Vec2 min(-5.0f);
    Vec2 max(5.0f);

    clamp_me.Clamp(min, max);
    clamp_me_2 = Vec2::Clamp(clamp_me_2, min, max);

    REQUIRE(clamp_me == result);
    REQUIRE(clamp_me_2 == result);
}

TEST_CASE("Vec2: Cross Functions")
{
    Vec2 foo(1.0f, 2.0f);
    Vec2 bar(2.0f, 1.0f);
    float result_1 = foo.Cross(bar);
    float result_2 = Vec2::Cross(foo, bar);
    float answer_1 = -3.0f;

    Vec2 result_3 = Vec2::Cross(1.0f, Vec2::UnitX());
    Vec2 answer_2 = Vec2::UnitY();

    Vec2 result_4 = Vec2::UnitX();
    result_4.Cross(1.0f);


    REQUIRE(result_1 == answer_1);
    REQUIRE(result_2 == answer_1);
    REQUIRE(result_3 == answer_2);
    REQUIRE(result_4 == -answer_2);
}

TEST_CASE("Vec2: Distance Functions")
{
    Vec2 foo(0.0f, 1.0f);
    Vec2 bar(1.0f, 0.0f);
    float answer_1 = math::SQRT_2;
    float result_1 = foo.Distance(bar);
    float result_2 = Vec2::Distance(foo, bar);

    float answer_2 = answer_1 * answer_1;
    float result_3 = foo.DistanceSquared(bar);
    float result_4 = Vec2::DistanceSquared(foo, bar);

    REQUIRE(math::IsEqual(answer_1, result_1));
    REQUIRE(math::IsEqual(answer_1, result_2));
    REQUIRE(math::IsEqual(answer_2, result_3));
    REQUIRE(math::IsEqual(answer_2, result_4));
}

TEST_CASE("Vec2: Dot Product Functions")
{
    Vec2 foo(1.0f, -1.0f);
    Vec2 bar(-1.0f, -1.0f);
    float result_1 = foo.Dot(bar);
    float result_2 = Vec2::Dot(foo, bar);
    float answer_1 = 0.0f;

    REQUIRE(answer_1 == result_1);
    REQUIRE(answer_1 == result_2);
}

TEST_CASE("Vec2: Length Functions")
{
    Vec2 foo(1.0f);
    float result_1 = foo.Length();
    float result_2 = Vec2::Length(foo);
    float answer_1 = math::SQRT_2;

    float result_3 = foo.LengthSquared();
    float result_4 = Vec2::LengthSquared(foo);
    float answer_2 = 2.0f;

    REQUIRE(math::IsEqual(answer_1, result_1));
    REQUIRE(math::IsEqual(answer_1, result_2));
    REQUIRE(math::IsEqual(answer_2, result_3));
    REQUIRE(math::IsEqual(answer_2, result_4));
}

TEST_CASE("Vec2: Angle Functions")
{
    Vec2 foo(1.0f);
    Vec2 bar(0.0f, 1.0f);
    float answer_1 = math::PI / 4.0f;
    float answer_2 = math::PI_DIV_2;
    float result_1 = foo.Angle();
    float result_2 = Vec2::Angle(foo);
    float result_3 = bar.Angle();
    float result_4 = Vec2::Angle(bar);

    Vec2 answer_3 = Vec2::Normalize(-foo);
    Vec2 result_5 = Vec2::AngleVec(math::PI + (math::PI / 4.0f));

    REQUIRE(answer_1 == result_1);
    REQUIRE(answer_1 == result_2);
    REQUIRE(answer_2 == result_3);
    REQUIRE(answer_2 == result_4);
    REQUIRE(answer_3 == result_5);
}

TEST_CASE("Vec2: Max & Min Functions")
{
    Vec2 max(1.0f);
    Vec2 min(-1.0f);

    Vec2 answer_1 = max;
    Vec2 answer_2 = min;
    Vec2 result_1 = min;
    Vec2 result_2 = max;
    result_1.Max(max);
    result_2.Min(min);

    Vec2 result_3 = Vec2::Max(min, max);
    Vec2 result_4 = Vec2::Min(min, max);

    REQUIRE(answer_1 == result_1);
    REQUIRE(answer_2 == result_2);
    REQUIRE(answer_1 == result_3);
    REQUIRE(answer_2 == result_4);
}

TEST_CASE("Vec2: Normalize Functions")
{
    Vec2 foo(0.0f, 2.0f);
    Vec2 bar = Vec2::Normalize(foo);
    Vec2 answer(0.0f, 1.0f);
    foo.Normalize();

    REQUIRE(foo == answer);
    REQUIRE(bar == answer);
}

TEST_CASE("Vec2: Reflect Functions")
{
    Vec2 incident(1.0f, -1.0f);
    Vec2 normal(0.0f, 1.0f);
    Vec2 answer(1.0f, 1.0f);
    Vec2 result_1 = Vec2::Reflect(incident, normal);
    Vec2 result_2 = incident;

    result_2.Reflect(normal);

    REQUIRE(result_1 == answer);
    REQUIRE(result_2 == answer);
}

TEST_CASE("Vec2: Comparing Direction Functions")
{
    Vec2 vec_1(1.0f, 1.0f);
    Vec2 vec_2(0.8f, 4.0f);

    Vec2 vec_3(-1.0f, 1.0f);
    Vec2 vec_4(1.0f, -1.0f);

    REQUIRE(vec_1.SameDirection(vec_2));
    REQUIRE(Vec2::SameDirection(vec_1, vec_2));
    REQUIRE(vec_3.OppositeDirection(vec_4));
    REQUIRE(Vec2::OppositeDirection(vec_3, vec_4));
}

TEST_CASE("SVec3: Clean Function")
{
    SVec3 clean_me(0.00001f);

    clean_me.Clean();

    REQUIRE(clean_me.X() == 0.0f);
}

TEST_CASE("SVec3: Set Function")
{
    SVec3 set_me;
    SVec3 set_me_2;

    set_me.Set(1.0f, 1.0f, 1.0f);
    set_me_2.Set(1.0f, 2.0f, 3.0f);

    REQUIRE(set_me == SVec3(1.0f));
    REQUIRE(set_me_2.X() == 1.0f);
    REQUIRE(set_me_2.Y() == 2.0f);
    REQUIRE(set_me_2.Z() == 3.0f);
}

TEST_CASE("SVec3: Clamp Functions")
{
    const SVec3 result(5.0f, -5.0f, 5.0f);
    SVec3 clamp_me(10.0f, -10.0f, 10.0f);
    SVec3 clamp_me_2(10.0f, -10.0f, 10.0f);
    SVec3 min(-5.0f);
    SVec3 max(5.0f);

    clamp_me.Clamp(min, max);
    clamp_me_2 = SVec3::Clamp(clamp_me_2, min, max);

    REQUIRE(clamp_me == result);
    REQUIRE(clamp_me_2 == result);
}

TEST_CASE("SVec3: Cross Functions")
{
    SVec3 foo = SVec3::UnitX();
    SVec3 bar = SVec3::UnitY();
    SVec3 foobar = SVec3::Cross(foo, bar);
    foo.Cross(bar);

    REQUIRE(foobar == SVec3::UnitZ());
    REQUIRE(foo == SVec3::UnitZ());
}

TEST_CASE("SVec3: Triple Product Function")
{
    SVec3 a(1.0f);
    float answer = 0.0f;
    float result = SVec3::TripleProduct(a, a, a);

    REQUIRE(answer == result);
}

TEST_CASE("SVec3: Distance Functions")
{
    SVec3 foo(0.0f, 1.0f, 0.0f);
    SVec3 bar(1.0f, 0.0f, 0.0f);
    float answer_1 = math::SQRT_2;
    float result_1 = foo.Distance(bar);
    float result_2 = SVec3::Distance(foo, bar);

    float answer_2 = answer_1 * answer_1;
    float result_3 = foo.DistanceSquared(bar);
    float result_4 = SVec3::DistanceSquared(foo, bar);

    REQUIRE(math::IsEqual(answer_1, result_1));
    REQUIRE(math::IsEqual(answer_1, result_2));
    REQUIRE(math::IsEqual(answer_2, result_3));
    REQUIRE(math::IsEqual(answer_2, result_4));
}

TEST_CASE("SVec3: Dot Product Functions")
{
    SVec3 foo(1.0f, 0.0f, 1.0f);
    SVec3 bar(0.0f, 1.0f, 0.0f);
    float result_1 = foo.Dot(bar);
    float result_2 = SVec3::Dot(foo, bar);
    float answer_1 = 0.0f;

    SVec3 harder(1.0f, 1.5f, 2.0f);
    SVec3 hey(1.0f);
    float result_3 = harder.Dot(hey);
    float answer_2 = 4.5f;

    REQUIRE(answer_1 == result_1);
    REQUIRE(answer_1 == result_2);
    REQUIRE(answer_2 == result_3);
}

TEST_CASE("SVec3: Length Functions")
{
    SVec3 foo(1.0f);
    float result_1 = foo.Length();
    float result_2 = SVec3::Length(foo);
    float answer_1 = 1.73205080757f;

    float result_3 = foo.LengthSquared();
    float result_4 = SVec3::LengthSquared(foo);
    float answer_2 = answer_1 * answer_1;

    REQUIRE(math::IsEqual(answer_1, result_1));
    REQUIRE(math::IsEqual(answer_1, result_2));
    REQUIRE(math::IsEqual(answer_2, result_3));
    REQUIRE(math::IsEqual(answer_2, result_4));
}

TEST_CASE("SVec3: Max & Min Functions")
{
    SVec3 max(1.0f);
    SVec3 min(-1.0f);

    SVec3 answer_1 = max;
    SVec3 answer_2 = min;
    SVec3 result_1 = min;
    SVec3 result_2 = max;
    result_1.Max(max);
    result_2.Min(min);

    SVec3 result_3 = SVec3::Max(min, max);
    SVec3 result_4 = SVec3::Min(min, max);

    REQUIRE(answer_1 == result_1);
    REQUIRE(answer_2 == result_2);
    REQUIRE(answer_1 == result_3);
    REQUIRE(answer_2 == result_4);
}

TEST_CASE("SVec3: Normalize Functions")
{
    SVec3 foo(0.0f, 2.0f, 0.0f);
    SVec3 bar = SVec3::Normalize(foo);
    SVec3 answer(0.0f, 1.0f, 0.0f);
    foo.Normalize();

    REQUIRE(foo == answer);
    REQUIRE(bar == answer);
}

TEST_CASE("SVec4: Clean Function")
{
    SVec4 clean_me(0.00001f);

    clean_me.Clean();

    REQUIRE(clean_me.X() == 0.0f);
}

TEST_CASE("SVec4: Set Function")
{
    SVec4 set_me;
    SVec4 set_me_2;

    set_me.Set(1.0f, 1.0f, 1.0f, 1.0f);
    set_me_2.Set(1.0f, 2.0f, 3.0f, 4.0f);

    REQUIRE(set_me == SVec4(1.0f));
    REQUIRE(set_me_2.X() == 1.0f);
    REQUIRE(set_me_2.Y() == 2.0f);
    REQUIRE(set_me_2.Z() == 3.0f);
    REQUIRE(set_me_2.W() == 4.0f);
}

TEST_CASE("SVec4: Clamp Functions")
{
    const SVec4 result(5.0f, -5.0f, 5.0f, -5.0f);
    SVec4 clamp_me(10.0f, -10.0f, 10.0f, -10.0f);
    SVec4 clamp_me_2(10.0f, -10.0f, 10.0f, -10.0f);
    SVec4 min(-5.0f);
    SVec4 max(5.0f);

    clamp_me.Clamp(min, max);
    clamp_me_2 = SVec4::Clamp(clamp_me_2, min, max);

    REQUIRE(clamp_me == result);
    REQUIRE(clamp_me_2 == result);
}

TEST_CASE("SVec4: Distance Functions")
{
    SVec4 foo(0.0f, 1.0f, 0.0f, 0.0f);
    SVec4 bar(1.0f, 0.0f, 0.0f, 0.0f);
    float answer_1 = math::SQRT_2;
    float result_1 = foo.Distance(bar);
    float result_2 = SVec4::Distance(foo, bar);

    float answer_2 = answer_1 * answer_1;
    float result_3 = foo.DistanceSquared(bar);
    float result_4 = SVec4::DistanceSquared(foo, bar);

    REQUIRE(math::IsEqual(answer_1, result_1));
    REQUIRE(math::IsEqual(answer_1, result_2));
    REQUIRE(math::IsEqual(answer_2, result_3));
    REQUIRE(math::IsEqual(answer_2, result_4));
}

TEST_CASE("SVec4: Dot Product Functions")
{
    SVec4 foo(1.0f, 0.0f, 1.0f, 0.0f);
    SVec4 bar(0.0f, 1.0f, 0.0f, 0.0f);
    float result_1 = foo.Dot(bar);
    float result_2 = SVec4::Dot(foo, bar);
    float answer_1 = 0.0f;

    SVec4 harder(0.5f, 1.0f, 2.0f, 3.0f);
    SVec4 hey(1.0f);
    float result_3 = harder.Dot(hey);
    float answer_2 = 6.5f;

    REQUIRE(answer_1 == result_1);
    REQUIRE(answer_1 == result_2);
    REQUIRE(answer_2 == result_3);
}

TEST_CASE("SVec4: Length Functions")
{
    SVec4 foo(1.0f);
    float result_1 = foo.Length();
    float result_2 = SVec4::Length(foo);
    float answer_1 = 2.0f;

    float result_3 = foo.LengthSquared();
    float result_4 = SVec4::LengthSquared(foo);
    float answer_2 = answer_1 * answer_1;

    REQUIRE(math::IsEqual(answer_1, result_1));
    REQUIRE(math::IsEqual(answer_1, result_2));
    REQUIRE(math::IsEqual(answer_2, result_3));
    REQUIRE(math::IsEqual(answer_2, result_4));
}

TEST_CASE("SVec4: Max & Min Functions")
{
    SVec4 max(1.0f);
    SVec4 min(-1.0f);

    SVec4 answer_1 = max;
    SVec4 answer_2 = min;
    SVec4 result_1 = min;
    SVec4 result_2 = max;
    result_1.Max(max);
    result_2.Min(min);

    SVec4 result_3 = SVec4::Max(min, max);
    SVec4 result_4 = SVec4::Min(min, max);

    REQUIRE(answer_1 == result_1);
    REQUIRE(answer_2 == result_2);
    REQUIRE(answer_1 == result_3);
    REQUIRE(answer_2 == result_4);
}

TEST_CASE("SVec4: Normalize Functions")
{
    SVec4 foo(0.0f, 2.0f, 0.0f, 0.0f);
    SVec4 bar = SVec4::Normalize(foo);
    SVec4 answer(0.0f, 1.0f, 0.0f, 0.0f);
    foo.Normalize();

    REQUIRE(foo == answer);
    REQUIRE(bar == answer);
}

/////////////////////////////////////////////////////////
// !Public Method Tests
/////////////////////////////////////////////////////////

TEST_CASE("Conversions")
{
	SVec4 hi1;
	Vec4 hi11;
	SVec3 hi2;
	Vec3 hi22;
	Vec2 hi3;

	SVec4 newhi1 = SVec4(hi11);
	newhi1 = SVec4(hi22, 2.0f);
	Vec4 newhi11 = Vec4(hi1);
	newhi11 = Vec4(hi2, 3.0f);

	SVec3 newhi2 = SVec3(hi22);
	newhi2 = SVec3(hi11);
	newhi2 = SVec3(hi3, 1.0f);
	Vec3 newhi22 = Vec3(hi2);
	newhi22 = Vec3(hi1);
	newhi22 = Vec3(hi3, 1.0f);

	Vec2 newhi3 = Vec2(hi22);
	newhi3 = Vec2(hi2);

	REQUIRE(true);
}
