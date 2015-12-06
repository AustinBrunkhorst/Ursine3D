/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MatrixLibTests.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#define CATCH_CONFIG_MAIN
#include "../dep/Testing/include/catch.hpp"

#include "SMat3.h"
#include "Mat3.h"
#include "SMat4.h"
#include "Mat4.h"

using namespace ursine;

/////////////////////////////////////////////////////////
// Construction Tests
/////////////////////////////////////////////////////////

TEST_CASE("SMat3: Default Constructor")
{
    SMat3 mat;

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: Copy Constructor")
{
    SMat3 foo;
    SMat3 mat(foo);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: SMat4 Constructor")
{
    SMat4 foo;
    SMat3 mat(foo);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: Row Constructor")
{
    SVec3 r0(1.0f, 2.0f, 3.0f);
    SVec3 r1(4.0f, 5.0f, 6.0f);
    SVec3 r2(7.0f, 8.0f, 9.0f);
    SMat3 mat(r0, r1, r2);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 3.0f);
    REQUIRE(mat(1, 0) == 4.0f); REQUIRE(mat(1, 1) == 5.0f); REQUIRE(mat(1, 2) == 6.0f);
    REQUIRE(mat(2, 0) == 7.0f); REQUIRE(mat(2, 1) == 8.0f); REQUIRE(mat(2, 2) == 9.0f);
}

TEST_CASE("SMat3: Individual Element Constructor")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 3.0f);
    REQUIRE(mat(1, 0) == 4.0f); REQUIRE(mat(1, 1) == 5.0f); REQUIRE(mat(1, 2) == 6.0f);
    REQUIRE(mat(2, 0) == 7.0f); REQUIRE(mat(2, 1) == 8.0f); REQUIRE(mat(2, 2) == 9.0f);
}

TEST_CASE("SMat3: 2D Rotation Constructor")
{
    SMat3 mat(45.0f);

    REQUIRE(mat(0, 0) == 0.707106781f); REQUIRE(mat(0, 1) == -0.707106781f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.707106781f); REQUIRE(mat(1, 1) == 0.707106781f);  REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f);         REQUIRE(mat(2, 1) == 0.0f);          REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: 2D Scalar Constructor")
{
    SMat3 mat(1.0f, 2.0f);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: 2D Translation Constructor")
{
    SMat3 mat(Vec2(1.0f, 2.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: 2D TRS Constructor")
{
    SMat3 mat(Vec2(1.0f, 2.0f), 45.0f, Vec2(1.0f, 2.0f));

    REQUIRE(math::IsEqual(mat(0, 0), sqrt(2.0f) / 2.0f)); REQUIRE(math::IsEqual(mat(0, 1), -sqrt(2.0f))); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(math::IsEqual(mat(1, 0), sqrt(2.0f) / 2.0f)); REQUIRE(math::IsEqual(mat(1, 1), sqrt(2.0f)));  REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f);                           REQUIRE(mat(2, 1) == 0.0f);                     REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: 3D Scale Constructor")
{
    SMat3 mat(SVec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f);
}

TEST_CASE("SMat3: 3D Euler Rotation Constructor")
{
    SMat3 mat(45.0f, 45.0f, 45.0f);

    REQUIRE(math::IsEqual(mat(0, 0), 0.853553f));  REQUIRE(math::IsEqual(mat(0, 1), -0.146447f)); REQUIRE(math::IsEqual(mat(0, 2), 0.5f));
    REQUIRE(math::IsEqual(mat(1, 0), 0.5f));       REQUIRE(math::IsEqual(mat(1, 1), 0.5f));       REQUIRE(math::IsEqual(mat(1, 2), -0.707107f));
    REQUIRE(math::IsEqual(mat(2, 0), -0.146447f)); REQUIRE(math::IsEqual(mat(2, 1), 0.853553f));  REQUIRE(math::IsEqual(mat(2, 2), 0.5f));
}

TEST_CASE("SMat4: Default Constructor")
{
    SMat4 mat;

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("SMat4: Copy Constructor")
{
    SMat4 foo;
    SMat4 mat(foo);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("SMat4: SMat3 Constructor")
{
    SMat3 foo;
    SMat4 mat(foo);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("SMat4: Row Constructor")
{
    SVec4 r0(1.0f, 2.0f, 3.0f, 4.0f);
    SVec4 r1(5.0f, 6.0f, 7.0f, 8.0f);
    SVec4 r2(9.0f, 10.0f, 11.0f, 12.0f);
    SVec4 r3(13.0f, 14.0f, 15.0f, 16.0f);
    SMat4 mat(r0, r1, r2, r3);

    REQUIRE(mat(0, 0) == 1.0f);  REQUIRE(mat(0, 1) == 2.0f);  REQUIRE(mat(0, 2) == 3.0f);  REQUIRE(mat(0, 3) == 4.0f);
    REQUIRE(mat(1, 0) == 5.0f);  REQUIRE(mat(1, 1) == 6.0f);  REQUIRE(mat(1, 2) == 7.0f);  REQUIRE(mat(1, 3) == 8.0f);
    REQUIRE(mat(2, 0) == 9.0f);  REQUIRE(mat(2, 1) == 10.0f); REQUIRE(mat(2, 2) == 11.0f); REQUIRE(mat(2, 3) == 12.0f);
    REQUIRE(mat(3, 0) == 13.0f); REQUIRE(mat(3, 1) == 14.0f); REQUIRE(mat(3, 2) == 15.0f); REQUIRE(mat(3, 3) == 16.0f);
}

TEST_CASE("SMat4: Individual Element Constructor")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);

    REQUIRE(mat(0, 0) == 1.0f);  REQUIRE(mat(0, 1) == 2.0f);  REQUIRE(mat(0, 2) == 3.0f);  REQUIRE(mat(0, 3) == 4.0f);
    REQUIRE(mat(1, 0) == 5.0f);  REQUIRE(mat(1, 1) == 6.0f);  REQUIRE(mat(1, 2) == 7.0f);  REQUIRE(mat(1, 3) == 8.0f);
    REQUIRE(mat(2, 0) == 9.0f);  REQUIRE(mat(2, 1) == 10.0f); REQUIRE(mat(2, 2) == 11.0f); REQUIRE(mat(2, 3) == 12.0f);
    REQUIRE(mat(3, 0) == 13.0f); REQUIRE(mat(3, 1) == 14.0f); REQUIRE(mat(3, 2) == 15.0f); REQUIRE(mat(3, 3) == 16.0f);
}

TEST_CASE("SMat4: 3D Scalar Constructor")
{
    SMat4 mat(1.0f, 2.0f, 3.0f);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("SMat4: 3D Translation Constructor")
{
    SMat4 mat(SVec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 1.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 3.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

/////////////////////////////////////////////////////////
// !Construction Tests
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// Accessor Tests
/////////////////////////////////////////////////////////

TEST_CASE("SMat3: Const and Mutable Functor")
{
    const SMat3 foo;

    // this line shouldn't compile
    // foo(0, 0) = 1.0f;
    REQUIRE(foo(0, 0) == 1.0f); REQUIRE(foo(0, 1) == 0.0f); REQUIRE(foo(0, 2) == 0.0f);
    REQUIRE(foo(1, 0) == 0.0f); REQUIRE(foo(1, 1) == 1.0f); REQUIRE(foo(1, 2) == 0.0f);
    REQUIRE(foo(2, 0) == 0.0f); REQUIRE(foo(2, 1) == 0.0f); REQUIRE(foo(2, 2) == 1.0f);

    SMat3 mat;

    for (uint i = 0; i < 3; ++i)
        for (uint j = 0; j < 3; ++j)
            mat(i, j) = 2.0f;

    REQUIRE(mat(0, 0) == 2.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 2.0f);
    REQUIRE(mat(1, 0) == 2.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 2.0f); REQUIRE(mat(2, 1) == 2.0f); REQUIRE(mat(2, 2) == 2.0f);
}

TEST_CASE("SMat3: Get Float Pointer")
{
    SMat3 mat;
    auto *ptr = mat.GetFloatPtr();

    // this line shouldn't compile
    // *ptr = 2.0f;

    void *addr_0 = &mat;
    void *addr_1 = const_cast<float*>(ptr);

    REQUIRE(addr_0 == addr_1);
}

TEST_CASE("SMat4: Const and Mutable Functor")
{
    const SMat4 foo;

    // this line shouldn't compile
    // foo(0, 0) = 1.0f;
    REQUIRE(foo(0, 0) == 1.0f); REQUIRE(foo(0, 1) == 0.0f); REQUIRE(foo(0, 2) == 0.0f); REQUIRE(foo(0, 3) == 0.0f);
    REQUIRE(foo(1, 0) == 0.0f); REQUIRE(foo(1, 1) == 1.0f); REQUIRE(foo(1, 2) == 0.0f); REQUIRE(foo(1, 3) == 0.0f);
    REQUIRE(foo(2, 0) == 0.0f); REQUIRE(foo(2, 1) == 0.0f); REQUIRE(foo(2, 2) == 1.0f); REQUIRE(foo(2, 3) == 0.0f);
    REQUIRE(foo(3, 0) == 0.0f); REQUIRE(foo(3, 1) == 0.0f); REQUIRE(foo(3, 2) == 0.0f); REQUIRE(foo(3, 3) == 1.0f);

    SMat4 mat;

    for (uint i = 0; i < 4; ++i)
        for (uint j = 0; j < 4; ++j)
            mat(i, j) = 2.0f;

    REQUIRE(mat(0, 0) == 2.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 2.0f); REQUIRE(mat(0, 3) == 2.0f);
    REQUIRE(mat(1, 0) == 2.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 2.0f); REQUIRE(mat(1, 3) == 2.0f);
    REQUIRE(mat(2, 0) == 2.0f); REQUIRE(mat(2, 1) == 2.0f); REQUIRE(mat(2, 2) == 2.0f); REQUIRE(mat(2, 3) == 2.0f);
    REQUIRE(mat(3, 0) == 2.0f); REQUIRE(mat(3, 1) == 2.0f); REQUIRE(mat(3, 2) == 2.0f); REQUIRE(mat(3, 3) == 2.0f);
}

TEST_CASE("SMat4: Get Float Pointer")
{
    SMat4 mat;
    auto *ptr = mat.GetFloatPtr();

    // this line shouldn't compile
    // *ptr = 2.0f;

    void *addr_0 = &mat;
    void *addr_1 = const_cast<float*>(ptr);

    REQUIRE(addr_0 == addr_1);
}

/////////////////////////////////////////////////////////
// !Accessor Tests
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// Operator Tests
/////////////////////////////////////////////////////////

TEST_CASE("SMat3: Matrix Multiplication Operator")
{
    SMat3 foo(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    SMat3 bar(foo);

    SMat3 mat = foo * bar;

    REQUIRE(mat(0, 0) == 30.0f);  REQUIRE(mat(0, 1) == 36.0f);  REQUIRE(mat(0, 2) == 42.0f);
    REQUIRE(mat(1, 0) == 66.0f);  REQUIRE(mat(1, 1) == 81.0f);  REQUIRE(mat(1, 2) == 96.0f);
    REQUIRE(mat(2, 0) == 102.0f); REQUIRE(mat(2, 1) == 126.0f); REQUIRE(mat(2, 2) == 150.0f);
}

TEST_CASE("SMat3: Matrix Self Multiplication Operator")
{
    SMat3 foo(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    SMat3 mat(foo);

    mat *= foo;

    REQUIRE(mat(0, 0) == 30.0f);  REQUIRE(mat(0, 1) == 36.0f);  REQUIRE(mat(0, 2) == 42.0f);
    REQUIRE(mat(1, 0) == 66.0f);  REQUIRE(mat(1, 1) == 81.0f);  REQUIRE(mat(1, 2) == 96.0f);
    REQUIRE(mat(2, 0) == 102.0f); REQUIRE(mat(2, 1) == 126.0f); REQUIRE(mat(2, 2) == 150.0f);

    mat = foo;
    mat *= mat;

    REQUIRE(mat(0, 0) == 30.0f);  REQUIRE(mat(0, 1) == 36.0f);  REQUIRE(mat(0, 2) == 42.0f);
    REQUIRE(mat(1, 0) == 66.0f);  REQUIRE(mat(1, 1) == 81.0f);  REQUIRE(mat(1, 2) == 96.0f);
    REQUIRE(mat(2, 0) == 102.0f); REQUIRE(mat(2, 1) == 126.0f); REQUIRE(mat(2, 2) == 150.0f);
}

TEST_CASE("SMat3: Matrix Scalar Multiplication Operator")
{
    SMat3 mat(1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f);

    mat = mat * 2.0f;

    REQUIRE(mat(0, 0) == 2.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 2.0f);
    REQUIRE(mat(1, 0) == 2.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 2.0f); REQUIRE(mat(2, 1) == 2.0f); REQUIRE(mat(2, 2) == 2.0f);

    mat *= 0.5f;

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 1.0f); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(mat(1, 0) == 1.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 1.0f);
    REQUIRE(mat(2, 0) == 1.0f); REQUIRE(mat(2, 1) == 1.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: Vector Multiplication Operator")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    SVec3 vec(1.0f, 2.0f, 3.0f);
    SVec3 result = mat * vec;

    REQUIRE(result.X() == 14.0f);
    REQUIRE(result.Y() == 32.0f);
    REQUIRE(result.Z() == 50.0f);
}

TEST_CASE("SMat3: Matrix Addition Operator")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    SMat3 foo(mat);

    mat = mat + foo;

    REQUIRE(mat(0, 0) == 2.0f); REQUIRE(mat(0, 1) == 4.0f); REQUIRE(mat(0, 2) == 6.0f);
    REQUIRE(mat(1, 0) == 8.0f); REQUIRE(mat(1, 1) == 10.0f); REQUIRE(mat(1, 2) == 12.0f);
    REQUIRE(mat(2, 0) == 14.0f); REQUIRE(mat(2, 1) == 16.0f); REQUIRE(mat(2, 2) == 18.0f);

    mat = foo;
    mat += mat;

    REQUIRE(mat(0, 0) == 2.0f); REQUIRE(mat(0, 1) == 4.0f); REQUIRE(mat(0, 2) == 6.0f);
    REQUIRE(mat(1, 0) == 8.0f); REQUIRE(mat(1, 1) == 10.0f); REQUIRE(mat(1, 2) == 12.0f);
    REQUIRE(mat(2, 0) == 14.0f); REQUIRE(mat(2, 1) == 16.0f); REQUIRE(mat(2, 2) == 18.0f);
}

TEST_CASE("SMat3: Matrix Subtraction Operator")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    SMat3 foo(mat);

    mat = mat - foo;

    REQUIRE(mat(0, 0) == 0.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 0.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 0.0f);

    mat = foo;
    mat -= mat;

    REQUIRE(mat(0, 0) == 0.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 0.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 0.0f);
}

TEST_CASE("SMat3: Matrix Comparison Operator")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    SMat3 foo(mat);

    REQUIRE(mat == foo);
}

TEST_CASE("SMat4: Matrix Multiplication Operator")
{
    SMat4 foo(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    SMat4 bar(foo);

    SMat4 mat = foo * bar;

    REQUIRE(mat(0, 0) == 90.0f);  REQUIRE(mat(0, 1) == 100.0f); REQUIRE(mat(0, 2) == 110.0f); REQUIRE(mat(0, 3) == 120.0f);
    REQUIRE(mat(1, 0) == 202.0f); REQUIRE(mat(1, 1) == 228.0f); REQUIRE(mat(1, 2) == 254.0f); REQUIRE(mat(1, 3) == 280.0f);
    REQUIRE(mat(2, 0) == 314.0f); REQUIRE(mat(2, 1) == 356.0f); REQUIRE(mat(2, 2) == 398.0f); REQUIRE(mat(2, 3) == 440.0f);
    REQUIRE(mat(3, 0) == 426.0f); REQUIRE(mat(3, 1) == 484.0f); REQUIRE(mat(3, 2) == 542.0f); REQUIRE(mat(3, 3) == 600.0f);
}

TEST_CASE("SMat4: Matrix Self Multiplication Operator")
{
    SMat4 foo(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    SMat4 mat(foo);

    mat *= foo;

    REQUIRE(mat(0, 0) == 90.0f);  REQUIRE(mat(0, 1) == 100.0f); REQUIRE(mat(0, 2) == 110.0f); REQUIRE(mat(0, 3) == 120.0f);
    REQUIRE(mat(1, 0) == 202.0f); REQUIRE(mat(1, 1) == 228.0f); REQUIRE(mat(1, 2) == 254.0f); REQUIRE(mat(1, 3) == 280.0f);
    REQUIRE(mat(2, 0) == 314.0f); REQUIRE(mat(2, 1) == 356.0f); REQUIRE(mat(2, 2) == 398.0f); REQUIRE(mat(2, 3) == 440.0f);
    REQUIRE(mat(3, 0) == 426.0f); REQUIRE(mat(3, 1) == 484.0f); REQUIRE(mat(3, 2) == 542.0f); REQUIRE(mat(3, 3) == 600.0f);

    mat = foo;
    mat *= mat;

    REQUIRE(mat(0, 0) == 90.0f);  REQUIRE(mat(0, 1) == 100.0f); REQUIRE(mat(0, 2) == 110.0f); REQUIRE(mat(0, 3) == 120.0f);
    REQUIRE(mat(1, 0) == 202.0f); REQUIRE(mat(1, 1) == 228.0f); REQUIRE(mat(1, 2) == 254.0f); REQUIRE(mat(1, 3) == 280.0f);
    REQUIRE(mat(2, 0) == 314.0f); REQUIRE(mat(2, 1) == 356.0f); REQUIRE(mat(2, 2) == 398.0f); REQUIRE(mat(2, 3) == 440.0f);
    REQUIRE(mat(3, 0) == 426.0f); REQUIRE(mat(3, 1) == 484.0f); REQUIRE(mat(3, 2) == 542.0f); REQUIRE(mat(3, 3) == 600.0f);
}

TEST_CASE("SMat4: Matrix Scalar Multiplication Operator")
{
    SMat4 mat(1.0f, 1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f, 1.0f);
    mat = mat * 2.0f;

    REQUIRE(mat(0, 0) == 2.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 2.0f); REQUIRE(mat(0, 3) == 2.0f);
    REQUIRE(mat(1, 0) == 2.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 2.0f); REQUIRE(mat(1, 3) == 2.0f);
    REQUIRE(mat(2, 0) == 2.0f); REQUIRE(mat(2, 1) == 2.0f); REQUIRE(mat(2, 2) == 2.0f); REQUIRE(mat(2, 3) == 2.0f);
    REQUIRE(mat(3, 0) == 2.0f); REQUIRE(mat(3, 1) == 2.0f); REQUIRE(mat(3, 2) == 2.0f); REQUIRE(mat(3, 3) == 2.0f);

    mat *= 0.5f;

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 1.0f); REQUIRE(mat(0, 2) == 1.0f); REQUIRE(mat(0, 3) == 1.0f);
    REQUIRE(mat(1, 0) == 1.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 1.0f); REQUIRE(mat(1, 3) == 1.0f);
    REQUIRE(mat(2, 0) == 1.0f); REQUIRE(mat(2, 1) == 1.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 1.0f);
    REQUIRE(mat(3, 0) == 1.0f); REQUIRE(mat(3, 1) == 1.0f); REQUIRE(mat(3, 2) == 1.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("SMat4: Vector Multiplication Operator")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    SVec4 vec(1.0f, 2.0f, 3.0f, 4.0f);
    SVec4 result = mat * vec;

    REQUIRE(result.X() == 30.0f);
    REQUIRE(result.Y() == 70.0f);
    REQUIRE(result.Z() == 110.0f);
    REQUIRE(result.W() == 150.0f);
}

TEST_CASE("SMat4: Matrix Addition Operator")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    SMat4 foo(mat);

    mat = mat + foo;

    REQUIRE(mat(0, 0) == 2.0f);  REQUIRE(mat(0, 1) == 4.0f);  REQUIRE(mat(0, 2) == 6.0f);  REQUIRE(mat(0, 3) == 8.0f);
    REQUIRE(mat(1, 0) == 10.0f); REQUIRE(mat(1, 1) == 12.0f); REQUIRE(mat(1, 2) == 14.0f); REQUIRE(mat(1, 3) == 16.0f);
    REQUIRE(mat(2, 0) == 18.0f); REQUIRE(mat(2, 1) == 20.0f); REQUIRE(mat(2, 2) == 22.0f); REQUIRE(mat(2, 3) == 24.0f);
    REQUIRE(mat(3, 0) == 26.0f); REQUIRE(mat(3, 1) == 28.0f); REQUIRE(mat(3, 2) == 30.0f); REQUIRE(mat(3, 3) == 32.0f);

    mat = foo;
    mat += mat;

    REQUIRE(mat(0, 0) == 2.0f);  REQUIRE(mat(0, 1) == 4.0f);  REQUIRE(mat(0, 2) == 6.0f);  REQUIRE(mat(0, 3) == 8.0f);
    REQUIRE(mat(1, 0) == 10.0f); REQUIRE(mat(1, 1) == 12.0f); REQUIRE(mat(1, 2) == 14.0f); REQUIRE(mat(1, 3) == 16.0f);
    REQUIRE(mat(2, 0) == 18.0f); REQUIRE(mat(2, 1) == 20.0f); REQUIRE(mat(2, 2) == 22.0f); REQUIRE(mat(2, 3) == 24.0f);
    REQUIRE(mat(3, 0) == 26.0f); REQUIRE(mat(3, 1) == 28.0f); REQUIRE(mat(3, 2) == 30.0f); REQUIRE(mat(3, 3) == 32.0f);
}

TEST_CASE("SMat4: Matrix Subtraction Operator")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    SMat4 foo(mat);

    mat = mat - foo;

    REQUIRE(mat(0, 0) == 0.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 0.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 0.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 0.0f);

    mat = foo;
    mat -= mat;

    REQUIRE(mat(0, 0) == 0.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 0.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 0.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 0.0f);
}

TEST_CASE("SMat4: Matrix Comparison Operator")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    SMat4 foo(mat);

    REQUIRE(mat == foo);
}

/////////////////////////////////////////////////////////
// !Operator Tests
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// Public Method Tests
/////////////////////////////////////////////////////////

TEST_CASE("SMat3: Clean Function")
{
    SMat3 mat(math::Epsilon, math::Epsilon, math::Epsilon,
             math::Epsilon, math::Epsilon, math::Epsilon,
             math::Epsilon, math::Epsilon, math::Epsilon);

    mat.Clean();

    REQUIRE(mat(0, 0) == 0.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 0.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 0.0f);
}

TEST_CASE("SMat3: Translate Function")
{
    SMat3 mat;

    mat.Translate(Vec2(1.0f, 2.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: Rotation Function")
{
    SMat3 mat;

    mat.Rotation(45.0f);

    REQUIRE(mat(0, 0) == 0.707106781f); REQUIRE(mat(0, 1) == -0.707106781f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.707106781f); REQUIRE(mat(1, 1) == 0.707106781f);  REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f);         REQUIRE(mat(2, 1) == 0.0f);          REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: Euler Rotation Function")
{
    SMat3 mat;

    mat.Rotation(45.0f, 45.0f, 45.0f);

    REQUIRE(math::IsEqual(mat(0, 0), 0.853553f));  REQUIRE(math::IsEqual(mat(0, 1), -0.146447f)); REQUIRE(math::IsEqual(mat(0, 2), 0.5f));
    REQUIRE(math::IsEqual(mat(1, 0), 0.5f));       REQUIRE(math::IsEqual(mat(1, 1), 0.5f));       REQUIRE(math::IsEqual(mat(1, 2), -0.707107f));
    REQUIRE(math::IsEqual(mat(2, 0), -0.146447f)); REQUIRE(math::IsEqual(mat(2, 1), 0.853553f));  REQUIRE(math::IsEqual(mat(2, 2), 0.5f));
}

TEST_CASE("SMat3: Scale Function")
{
    SMat3 mat;

    mat.Scale(Vec2(1.0f, 2.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);

    mat.Scale(SVec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f);
}

TEST_CASE("SMat3: TRS Function")
{
    SMat3 mat;

    mat.TRS(Vec2(1.0f, 2.0f), 45.0f, Vec2(1.0f, 2.0f));

    REQUIRE(math::IsEqual(mat(0, 0), sqrt(2.0f) / 2.0f)); REQUIRE(math::IsEqual(mat(0, 1), -sqrt(2.0f))); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(math::IsEqual(mat(1, 0), sqrt(2.0f) / 2.0f)); REQUIRE(math::IsEqual(mat(1, 1), sqrt(2.0f)));  REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f);                           REQUIRE(mat(2, 1) == 0.0f);                     REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("SMat3: Transpose Function")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    const SMat3 const_mat(mat);
    SMat3 result(1.0f, 4.0f, 7.0f,
                2.0f, 5.0f, 8.0f,
                3.0f, 6.0f, 9.0f);

    mat.Transpose();
    SMat3 answer = const_mat.Transpose();

    REQUIRE(mat == result);
    REQUIRE(answer == result);
}

TEST_CASE("SMat3: Inverse Function")
{
    SMat3 mat(10.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    const SMat3 const_mat(mat);
    SMat3 answer(1.0f/9.0f, -2.0f/9.0f, 1.0f/9.0f,
                -2.0f/9.0f, -23.0f/9.0f, 16.0f/9.0f,
                1.0f/9.0f, 22.0f/9.0f, -14.0f/9.0f);

    mat.Inverse();
    SMat3 result = const_mat.Inverse();

    REQUIRE(mat == answer);
    REQUIRE(result == answer);
}

TEST_CASE("SMat3: Determinant Function")
{
    SMat3 mat(10.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);

    float answer = -27.0f;
    float result = mat.Determinant();

    REQUIRE(answer == result);
}

TEST_CASE("SMat3: Set Rows Function")
{
    SMat3 mat;

    mat.SetRows(SVec3(1.0f), SVec3(2.0f), SVec3(3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 1.0f); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(mat(1, 0) == 2.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 3.0f); REQUIRE(mat(2, 1) == 3.0f); REQUIRE(mat(2, 2) == 3.0f);
}

TEST_CASE("SMat3: Get Rows Function")
{
    SMat3 mat(1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f);

    SVec3 r0, r1, r2;

    mat.GetRows(r0, r1, r2);

    REQUIRE(r0 == SVec3(1.0f));
    REQUIRE(r1 == SVec3(1.0f));
    REQUIRE(r2 == SVec3(1.0f));
}

TEST_CASE("SMat3: Set Row Function")
{
    SMat3 mat;

    mat.SetRow(0, SVec3(1.0f));
    mat.SetRow(1, SVec3(2.0f));
    mat.SetRow(2, SVec3(3.0f));

    SMat3 answer;
    answer.SetRows(SVec3(1.0f), SVec3(2.0f), SVec3(3.0f));

    REQUIRE(answer == mat);
}

TEST_CASE("SMat3: Get Row Function")
{
    SMat3 mat;

    mat.SetRows(SVec3(1.0f), SVec3(2.0f), SVec3(3.0f));

    auto r0 = mat.GetRow(0);
    auto r1 = mat.GetRow(1);
    auto r2 = mat.GetRow(2);

    REQUIRE(r0 == SVec3(1.0f));
    REQUIRE(r1 == SVec3(2.0f));
    REQUIRE(r2 == SVec3(3.0f));
}

TEST_CASE("SMat3: Set Columns Function")
{
    SMat3 mat;

    mat.SetColumns(SVec3(1.0f), SVec3(2.0f), SVec3(3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 3.0f);
    REQUIRE(mat(1, 0) == 1.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 3.0f);
    REQUIRE(mat(2, 0) == 1.0f); REQUIRE(mat(2, 1) == 2.0f); REQUIRE(mat(2, 2) == 3.0f);
}

TEST_CASE("SMat3: Get Columns Function")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             1.0f, 2.0f, 3.0f,
             1.0f, 2.0f, 3.0f);

    SVec3 c0, c1, c2;

    mat.GetColumns(c0, c1, c2);

    REQUIRE(c0 == SVec3(1.0f));
    REQUIRE(c1 == SVec3(2.0f));
    REQUIRE(c2 == SVec3(3.0f));
}

TEST_CASE("SMat3: Set Column Function")
{
    SMat3 mat;

    mat.SetColumn(0, SVec3(1.0f));
    mat.SetColumn(1, SVec3(2.0f));
    mat.SetColumn(2, SVec3(3.0f));

    SVec3 c0, c1, c2;

    mat.GetColumns(c0, c1, c2);

    REQUIRE(c0 == SVec3(1.0f));
    REQUIRE(c1 == SVec3(2.0f));
    REQUIRE(c2 == SVec3(3.0f));
}

TEST_CASE("SMat3: Get Column Function")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             1.0f, 2.0f, 3.0f,
             1.0f, 2.0f, 3.0f);

    SVec3 c0, c1, c2;

    c0 = mat.GetColumn(0);
    c1 = mat.GetColumn(1);
    c2 = mat.GetColumn(2);

    REQUIRE(c0 == SVec3(1.0f));
    REQUIRE(c1 == SVec3(2.0f));
    REQUIRE(c2 == SVec3(3.0f));
}

TEST_CASE("SMat3: Transform Vector Function")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             0.0f, 0.0f, 1.0f);
    Vec2 vec(1.0f, 2.0f);
    Vec2 answer(5.0f, 14.0f);

    Vec2 result = mat.TransformVector(vec);
    mat.TransformVectorInplace(vec);

    REQUIRE(result == answer);
    REQUIRE(vec == answer);
}

TEST_CASE("SMat3: Transform Point Function")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             0.0f, 0.0f, 1.0f);
    Vec2 point(1.0f, 2.0f);
    Vec2 answer(8.0f, 20.0f);

    Vec2 result = mat.TransformPoint(point);
    mat.TransformPointInplace(point);

    REQUIRE(result == answer);
    REQUIRE(point == answer);
}

TEST_CASE("SMat3: Transform Point And Divide Function")
{
    SMat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    Vec2 point(1.0f, 2.0f);
    Vec2 answer(1.0f/4.0f, 5.0f/8.0f);

    Vec2 result = mat.TransformPointAndDiv(point);
    mat.TransformPointAndDivInplace(point);

    REQUIRE(result == answer);
    REQUIRE(point == answer);
}

TEST_CASE("SMat4: Clean Function")
{
    SMat4 mat(math::Epsilon, math::Epsilon, math::Epsilon, math::Epsilon,
             math::Epsilon, math::Epsilon, math::Epsilon, math::Epsilon,
             math::Epsilon, math::Epsilon, math::Epsilon, math::Epsilon,
             math::Epsilon, math::Epsilon, math::Epsilon, math::Epsilon);

    mat.Clean();

    REQUIRE(mat(0, 0) == 0.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 0.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 0.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 0.0f);
}

TEST_CASE("SMat4: Translation Function")
{
    SMat4 mat;

    mat.Translate(SVec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 1.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 3.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("SMat4: Euler Rotation Function")
{
    SMat4 mat;

    mat.Rotation(45.0f, 45.0f, 45.0f);

    REQUIRE(math::IsEqual(mat(0, 0), 0.853553f));  REQUIRE(math::IsEqual(mat(0, 1), -0.146447f)); REQUIRE(math::IsEqual(mat(0, 2), 0.5f));       REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(math::IsEqual(mat(1, 0), 0.5f));       REQUIRE(math::IsEqual(mat(1, 1), 0.5f));       REQUIRE(math::IsEqual(mat(1, 2), -0.707107f)); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(math::IsEqual(mat(2, 0), -0.146447f)); REQUIRE(math::IsEqual(mat(2, 1), 0.853553f));  REQUIRE(math::IsEqual(mat(2, 2), 0.5f));       REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("SMat4: Scalar Function")
{
    SMat4 mat;

    mat.Scale(SVec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);

    mat.Scale(SVec4(1.0f, 2.0f, 3.0f, 4.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 4.0f);
}

TEST_CASE("SMat4: Tranpose Functions")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    const SMat4 const_mat(mat);
    SMat4 answer(1.0f, 5.0f, 9.0f, 13.0f,
                2.0f, 6.0f, 10.0f, 14.0f,
                3.0f, 7.0f, 11.0f, 15.0f,
                4.0f, 8.0f, 12.0f, 16.0f);

    mat.Transpose();
    SMat4 result = const_mat.Transpose();

    REQUIRE(mat == answer);
    REQUIRE(result == answer);
}

TEST_CASE("SMat4: Inverse Functions")
{
    SMat4 mat(10.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 60.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    const SMat4 const_mat(mat);
    SMat4 answer(1.0f/9.0f, 0.0f, -1.0f/3.0f, 2.0f/9.0f,
                0.0f, 1.0f/54.0f, -1.0f/27.0f, 1.0f/54.0f,
                -1.0f/3.0f, -1.0f/27.0f, -79.0f/27.0f, 62.0f/27.0f,
                2.0f/9.0f, 1.0f/54.0f, 329.0f/108.0f, -247.0f/108.0f);

    mat.Inverse();
    SMat4 result = const_mat.Inverse();

    REQUIRE(answer == mat);
    REQUIRE(answer == result);
}

TEST_CASE("SMat4: Inverse Fast Function")
{
    /*SMat4 foo = SMat4(SVec3(1.0f, 2.0f, 3.0f)) * SMat4(1.0f, 2.0f, 3.0f);
    SMat4 bar;
    bar.RotationZXY(45.0f, 45.0f, 45.0f);
    SMat4 mat = foo * bar;
    const SMat4 const_mat(mat);
    SMat4 answer(0.853553f, 0.25f, -0.048816f, -1.20711f,
                -0.146447f, 0.25f, 0.284518f, -1.20711f,
                0.5f, -0.353553f, 0.166667f, -0.292893f,
                0.0f, 0.0f, 0.0f, 1.0f);

    mat.InverseFast();
    SMat4 result = const_mat.InverseFast();

    REQUIRE(mat == answer);
    REQUIRE(result == answer);*/
}

TEST_CASE("SMat4: Determinant Function")
{
    SMat4 mat(10.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 60.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);

    float det = mat.Determinant();

    REQUIRE(det == -1944.0f);
}

TEST_CASE("SMat4: Set Rows Function")
{
    SMat4 mat;

    mat.SetRows(SVec4(1.0f), SVec4(2.0f), SVec4(3.0f), SVec4(4.0f));

    SMat4 result(SVec4(1.0f), SVec4(2.0f), SVec4(3.0f), SVec4(4.0f));

    REQUIRE(mat == result);
}

TEST_CASE("SMat4: Get Rows Function")
{
    SMat4 mat(SVec4(1.0f), SVec4(2.0f), SVec4(3.0f), SVec4(4.0f));

    SVec4 r0, r1, r2, r3;

    mat.GetRows(r0, r1, r2, r3);

    REQUIRE(r0 == SVec4(1.0f));
    REQUIRE(r1 == SVec4(2.0f));
    REQUIRE(r2 == SVec4(3.0f));
    REQUIRE(r3 == SVec4(4.0f));
}

TEST_CASE("SMat4: Set Row Function")
{
    SMat4 mat;

    mat.SetRow(0, SVec4(1.0f));
    mat.SetRow(1, SVec4(2.0f));
    mat.SetRow(2, SVec4(3.0f));
    mat.SetRow(3, SVec4(4.0f));

    SVec4 r0, r1, r2, r3;

    mat.GetRows(r0, r1, r2, r3);

    REQUIRE(r0 == SVec4(1.0f));
    REQUIRE(r1 == SVec4(2.0f));
    REQUIRE(r2 == SVec4(3.0f));
    REQUIRE(r3 == SVec4(4.0f));
}

TEST_CASE("SMat4: Get Row Function")
{
    SMat4 mat(SVec4(1.0f), SVec4(2.0f), SVec4(3.0f), SVec4(4.0f));

    SVec4 r0, r1, r2, r3;

    r0 = mat.GetRow(0);
    r1 = mat.GetRow(1);
    r2 = mat.GetRow(2);
    r3 = mat.GetRow(3);

    REQUIRE(r0 == SVec4(1.0f));
    REQUIRE(r1 == SVec4(2.0f));
    REQUIRE(r2 == SVec4(3.0f));
    REQUIRE(r3 == SVec4(4.0f));
}

TEST_CASE("SMat4: Set Columns Function")
{
    SMat4 mat;

    mat.SetColumns(SVec4(1.0f), SVec4(2.0f), SVec4(3.0f), SVec4(4.0f));

    SMat4 result(1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f);

    REQUIRE(mat == result);
}

TEST_CASE("SMat4: Get Columns Function")
{
    SMat4 mat;

    mat.SetColumns(SVec4(1.0f), SVec4(2.0f), SVec4(3.0f), SVec4(4.0f));

    SVec4 c0, c1, c2, c3;

    mat.GetColumns(c0, c1, c2, c3);

    REQUIRE(c0 == SVec4(1.0f));
    REQUIRE(c1 == SVec4(2.0f));
    REQUIRE(c2 == SVec4(3.0f));
    REQUIRE(c3 == SVec4(4.0f));
}

TEST_CASE("SMat4: Set Column Function")
{
    SMat4 mat;

    mat.SetColumn(0, SVec4(1.0f));
    mat.SetColumn(1, SVec4(2.0f));
    mat.SetColumn(2, SVec4(3.0f));
    mat.SetColumn(3, SVec4(4.0f));

    SMat4 result(1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f);

    REQUIRE(mat == result);
}

TEST_CASE("SMat4: Get Column Function")
{
    SMat4 mat;

    mat.SetColumns(SVec4(1.0f), SVec4(2.0f), SVec4(3.0f), SVec4(4.0f));

    SVec4 c0, c1, c2, c3;

    c0 = mat.GetColumn(0);
    c1 = mat.GetColumn(1);
    c2 = mat.GetColumn(2);
    c3 = mat.GetColumn(3);

    REQUIRE(c0 == SVec4(1.0f));
    REQUIRE(c1 == SVec4(2.0f));
    REQUIRE(c2 == SVec4(3.0f));
    REQUIRE(c3 == SVec4(4.0f));
}

TEST_CASE("SMat4: Transform Vector Function")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             0.0f, 0.0f, 0.0f, 1.0f);
    SVec3 vec(1.0f, 2.0f, 3.0f);
    SVec3 answer(14.0f, 38.0f, 62.0f);

    SVec3 result = mat.TransformVector(vec);
    mat.TransformVectorInplace(vec);

    REQUIRE(result == answer);
    REQUIRE(vec == answer);
}

TEST_CASE("SMat4: Transform Point Function")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             0.0f, 0.0f, 0.0f, 1.0f);
    SVec3 point(1.0f, 2.0f, 3.0f);
    SVec3 answer(18.0f, 46.0f, 74.0f);

    SVec3 result = mat.TransformPoint(point);
    mat.TransformPointInplace(point);

    REQUIRE(result == answer);
    REQUIRE(point == answer);
}

TEST_CASE("SMat4: Transform Point And Divide Function")
{
    SMat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    SVec3 point(1.0f, 2.0f, 3.0f);
    SVec3 answer(3.0f/17.0f, 23.0f/51.0f, 37.0f/51.0f);

    SVec3 result = mat.TransformPointAndDiv(point);
    mat.TransformPointAndDivInplace(point);

    REQUIRE(result == answer);
    REQUIRE(point == answer);
}

/////////////////////////////////////////////////////////
// !Public Method Tests
/////////////////////////////////////////////////////////

TEST_CASE("Conversions")
{
	SMat4 smat4;
	Mat4 mat4;
	SMat3 smat3;
	Mat3 mat3;

	smat4 = SMat4(mat4);
	smat4 = SMat4(mat3);
	smat4 = SMat4(smat3);

	mat4 = Mat4(smat4);
	mat4 = Mat4(smat3);
	mat4 = Mat4(mat3);

	smat3 = SMat3(mat3);
	smat3 = SMat3(mat4);
	smat3 = SMat3(smat4);

	mat3 = Mat3(smat3);
	mat3 = Mat3(smat4);
	mat3 = Mat3(mat4);
}
