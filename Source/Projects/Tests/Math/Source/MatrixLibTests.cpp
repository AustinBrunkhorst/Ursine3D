#define CATCH_CONFIG_MAIN
#include "../../../Testing/catch.hpp"

#include "../NewMath/Mat3.h"
#include "../NewMath/Mat4.h"

using namespace Ursine;

/////////////////////////////////////////////////////////
// Construction Tests
/////////////////////////////////////////////////////////

TEST_CASE("Mat3: Default Constructor")
{
    Mat3 mat;

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: Copy Constructor")
{
    Mat3 foo;
    Mat3 mat(foo);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: Mat4 Constructor")
{
    Mat4 foo;
    Mat3 mat(foo);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: Row Constructor")
{
    Vec3 r0(1.0f, 2.0f, 3.0f);
    Vec3 r1(4.0f, 5.0f, 6.0f);
    Vec3 r2(7.0f, 8.0f, 9.0f);
    Mat3 mat(r0, r1, r2);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 3.0f);
    REQUIRE(mat(1, 0) == 4.0f); REQUIRE(mat(1, 1) == 5.0f); REQUIRE(mat(1, 2) == 6.0f);
    REQUIRE(mat(2, 0) == 7.0f); REQUIRE(mat(2, 1) == 8.0f); REQUIRE(mat(2, 2) == 9.0f);
}

TEST_CASE("Mat3: Individual Element Constructor")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 3.0f);
    REQUIRE(mat(1, 0) == 4.0f); REQUIRE(mat(1, 1) == 5.0f); REQUIRE(mat(1, 2) == 6.0f);
    REQUIRE(mat(2, 0) == 7.0f); REQUIRE(mat(2, 1) == 8.0f); REQUIRE(mat(2, 2) == 9.0f);
}

TEST_CASE("Mat3: 2D Rotation Constructor")
{
    Mat3 mat(Math::PI / 4.0f);

    REQUIRE(mat(0, 0) == 0.707106781f); REQUIRE(mat(0, 1) == -0.707106781f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.707106781f); REQUIRE(mat(1, 1) == 0.707106781f);  REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f);         REQUIRE(mat(2, 1) == 0.0f);          REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: 2D Scalar Constructor")
{
    Mat3 mat(1.0f, 2.0f);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: 2D Translation Constructor")
{
    Mat3 mat(Vec2(1.0f, 2.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: 2D TRS Constructor")
{
    Mat3 mat(Vec2(1.0f, 2.0f), Math::PI / 4.0f, Vec2(1.0f, 2.0f));

    REQUIRE(Math::IsEqual(mat(0, 0), sqrt(2.0f) / 2.0f)); REQUIRE(Math::IsEqual(mat(0, 1), -sqrt(2.0f))); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(Math::IsEqual(mat(1, 0), sqrt(2.0f) / 2.0f)); REQUIRE(Math::IsEqual(mat(1, 1), sqrt(2.0f)));  REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f);                           REQUIRE(mat(2, 1) == 0.0f);                     REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: 3D Scale Constructor")
{
    Mat3 mat(Vec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f);
}

TEST_CASE("Mat3: 3D Euler Rotation Constructor")
{
    Mat3 mat(Math::PI/4.0f, Math::PI/4.0f, Math::PI/4.0f);

    REQUIRE(Math::IsEqual(mat(0, 0), 0.853553f));  REQUIRE(Math::IsEqual(mat(0, 1), -0.146447f)); REQUIRE(Math::IsEqual(mat(0, 2), 0.5f));
    REQUIRE(Math::IsEqual(mat(1, 0), 0.5f));       REQUIRE(Math::IsEqual(mat(1, 1), 0.5f));       REQUIRE(Math::IsEqual(mat(1, 2), -0.707107f));
    REQUIRE(Math::IsEqual(mat(2, 0), -0.146447f)); REQUIRE(Math::IsEqual(mat(2, 1), 0.853553f));  REQUIRE(Math::IsEqual(mat(2, 2), 0.5f));
}

TEST_CASE("Mat4: Default Constructor")
{
    Mat4 mat;

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("Mat4: Copy Constructor")
{
    Mat4 foo;
    Mat4 mat(foo);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("Mat4: Mat3 Constructor")
{
    Mat3 foo;
    Mat4 mat(foo);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("Mat4: Row Constructor")
{
    Vec4 r0(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 r1(5.0f, 6.0f, 7.0f, 8.0f);
    Vec4 r2(9.0f, 10.0f, 11.0f, 12.0f);
    Vec4 r3(13.0f, 14.0f, 15.0f, 16.0f);
    Mat4 mat(r0, r1, r2, r3);

    REQUIRE(mat(0, 0) == 1.0f);  REQUIRE(mat(0, 1) == 2.0f);  REQUIRE(mat(0, 2) == 3.0f);  REQUIRE(mat(0, 3) == 4.0f);
    REQUIRE(mat(1, 0) == 5.0f);  REQUIRE(mat(1, 1) == 6.0f);  REQUIRE(mat(1, 2) == 7.0f);  REQUIRE(mat(1, 3) == 8.0f);
    REQUIRE(mat(2, 0) == 9.0f);  REQUIRE(mat(2, 1) == 10.0f); REQUIRE(mat(2, 2) == 11.0f); REQUIRE(mat(2, 3) == 12.0f);
    REQUIRE(mat(3, 0) == 13.0f); REQUIRE(mat(3, 1) == 14.0f); REQUIRE(mat(3, 2) == 15.0f); REQUIRE(mat(3, 3) == 16.0f);
}

TEST_CASE("Mat4: Individual Element Constructor")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);

    REQUIRE(mat(0, 0) == 1.0f);  REQUIRE(mat(0, 1) == 2.0f);  REQUIRE(mat(0, 2) == 3.0f);  REQUIRE(mat(0, 3) == 4.0f);
    REQUIRE(mat(1, 0) == 5.0f);  REQUIRE(mat(1, 1) == 6.0f);  REQUIRE(mat(1, 2) == 7.0f);  REQUIRE(mat(1, 3) == 8.0f);
    REQUIRE(mat(2, 0) == 9.0f);  REQUIRE(mat(2, 1) == 10.0f); REQUIRE(mat(2, 2) == 11.0f); REQUIRE(mat(2, 3) == 12.0f);
    REQUIRE(mat(3, 0) == 13.0f); REQUIRE(mat(3, 1) == 14.0f); REQUIRE(mat(3, 2) == 15.0f); REQUIRE(mat(3, 3) == 16.0f);
}

TEST_CASE("Mat4: 3D Scalar Constructor")
{
    Mat4 mat(1.0f, 2.0f, 3.0f);

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("Mat4: 3D Translation Constructor")
{
    Mat4 mat(Vec3(1.0f, 2.0f, 3.0f));

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

TEST_CASE("Mat3: Const and Mutable Functor")
{
    const Mat3 foo;

    // this line shouldn't compile
    // foo(0, 0) = 1.0f;
    REQUIRE(foo(0, 0) == 1.0f); REQUIRE(foo(0, 1) == 0.0f); REQUIRE(foo(0, 2) == 0.0f);
    REQUIRE(foo(1, 0) == 0.0f); REQUIRE(foo(1, 1) == 1.0f); REQUIRE(foo(1, 2) == 0.0f);
    REQUIRE(foo(2, 0) == 0.0f); REQUIRE(foo(2, 1) == 0.0f); REQUIRE(foo(2, 2) == 1.0f);

    Mat3 mat;

    for (uint i = 0; i < 3; ++i)
        for (uint j = 0; j < 3; ++j)
            mat(i, j) = 2.0f;

    REQUIRE(mat(0, 0) == 2.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 2.0f);
    REQUIRE(mat(1, 0) == 2.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 2.0f); REQUIRE(mat(2, 1) == 2.0f); REQUIRE(mat(2, 2) == 2.0f);
}

TEST_CASE("Mat3: Get Float Pointer")
{
    Mat3 mat;
    auto *ptr = mat.GetFloatPtr();

    // this line shouldn't compile
    // *ptr = 2.0f;

    void *addr_0 = &mat;
    void *addr_1 = const_cast<float*>(ptr);

    REQUIRE(addr_0 == addr_1);
}

TEST_CASE("Mat4: Const and Mutable Functor")
{
    const Mat4 foo;

    // this line shouldn't compile
    // foo(0, 0) = 1.0f;
    REQUIRE(foo(0, 0) == 1.0f); REQUIRE(foo(0, 1) == 0.0f); REQUIRE(foo(0, 2) == 0.0f); REQUIRE(foo(0, 3) == 0.0f);
    REQUIRE(foo(1, 0) == 0.0f); REQUIRE(foo(1, 1) == 1.0f); REQUIRE(foo(1, 2) == 0.0f); REQUIRE(foo(1, 3) == 0.0f);
    REQUIRE(foo(2, 0) == 0.0f); REQUIRE(foo(2, 1) == 0.0f); REQUIRE(foo(2, 2) == 1.0f); REQUIRE(foo(2, 3) == 0.0f);
    REQUIRE(foo(3, 0) == 0.0f); REQUIRE(foo(3, 1) == 0.0f); REQUIRE(foo(3, 2) == 0.0f); REQUIRE(foo(3, 3) == 1.0f);

    Mat4 mat;

    for (uint i = 0; i < 4; ++i)
        for (uint j = 0; j < 4; ++j)
            mat(i, j) = 2.0f;

    REQUIRE(mat(0, 0) == 2.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 2.0f); REQUIRE(mat(0, 3) == 2.0f);
    REQUIRE(mat(1, 0) == 2.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 2.0f); REQUIRE(mat(1, 3) == 2.0f);
    REQUIRE(mat(2, 0) == 2.0f); REQUIRE(mat(2, 1) == 2.0f); REQUIRE(mat(2, 2) == 2.0f); REQUIRE(mat(2, 3) == 2.0f);
    REQUIRE(mat(3, 0) == 2.0f); REQUIRE(mat(3, 1) == 2.0f); REQUIRE(mat(3, 2) == 2.0f); REQUIRE(mat(3, 3) == 2.0f);
}

TEST_CASE("Mat4: Get Float Pointer")
{
    Mat4 mat;
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

TEST_CASE("Mat3: Matrix Multiplication Operator")
{
    Mat3 foo(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    Mat3 bar(foo);

    Mat3 mat = foo * bar;

    REQUIRE(mat(0, 0) == 30.0f);  REQUIRE(mat(0, 1) == 36.0f);  REQUIRE(mat(0, 2) == 42.0f);
    REQUIRE(mat(1, 0) == 66.0f);  REQUIRE(mat(1, 1) == 81.0f);  REQUIRE(mat(1, 2) == 96.0f);
    REQUIRE(mat(2, 0) == 102.0f); REQUIRE(mat(2, 1) == 126.0f); REQUIRE(mat(2, 2) == 150.0f);
}

TEST_CASE("Mat3: Matrix Self Multiplication Operator")
{
    Mat3 foo(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    Mat3 mat(foo);

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

TEST_CASE("Mat3: Matrix Scalar Multiplication Operator")
{
    Mat3 mat(1.0f, 1.0f, 1.0f,
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

TEST_CASE("Mat3: Vector Multiplication Operator")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    Vec3 vec(1.0f, 2.0f, 3.0f);
    Vec3 result = mat * vec;

    REQUIRE(result.X() == 14.0f);
    REQUIRE(result.Y() == 32.0f);
    REQUIRE(result.Z() == 50.0f);
}

TEST_CASE("Mat3: Matrix Addition Operator")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    Mat3 foo(mat);

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

TEST_CASE("Mat3: Matrix Subtraction Operator")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    Mat3 foo(mat);

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

TEST_CASE("Mat3: Matrix Comparison Operator")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    Mat3 foo(mat);

    REQUIRE(mat == foo);
}

TEST_CASE("Mat4: Matrix Multiplication Operator")
{
    Mat4 foo(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    Mat4 bar(foo);

    Mat4 mat = foo * bar;

    REQUIRE(mat(0, 0) == 90.0f);  REQUIRE(mat(0, 1) == 100.0f); REQUIRE(mat(0, 2) == 110.0f); REQUIRE(mat(0, 3) == 120.0f);
    REQUIRE(mat(1, 0) == 202.0f); REQUIRE(mat(1, 1) == 228.0f); REQUIRE(mat(1, 2) == 254.0f); REQUIRE(mat(1, 3) == 280.0f);
    REQUIRE(mat(2, 0) == 314.0f); REQUIRE(mat(2, 1) == 356.0f); REQUIRE(mat(2, 2) == 398.0f); REQUIRE(mat(2, 3) == 440.0f);
    REQUIRE(mat(3, 0) == 426.0f); REQUIRE(mat(3, 1) == 484.0f); REQUIRE(mat(3, 2) == 542.0f); REQUIRE(mat(3, 3) == 600.0f);
}

TEST_CASE("Mat4: Matrix Self Multiplication Operator")
{
    Mat4 foo(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    Mat4 mat(foo);

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

TEST_CASE("Mat4: Matrix Scalar Multiplication Operator")
{
    Mat4 mat(1.0f, 1.0f, 1.0f, 1.0f,
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

TEST_CASE("Mat4: Vector Multiplication Operator")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    Vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 result = mat * vec;

    REQUIRE(result.X() == 30.0f);
    REQUIRE(result.Y() == 70.0f);
    REQUIRE(result.Z() == 110.0f);
    REQUIRE(result.W() == 150.0f);
}

TEST_CASE("Mat4: Matrix Addition Operator")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    Mat4 foo(mat);

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

TEST_CASE("Mat4: Matrix Subtraction Operator")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    Mat4 foo(mat);

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

TEST_CASE("Mat4: Matrix Comparison Operator")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    Mat4 foo(mat);

    REQUIRE(mat == foo);
}

/////////////////////////////////////////////////////////
// !Operator Tests
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// Public Method Tests
/////////////////////////////////////////////////////////

TEST_CASE("Mat3: Clean Function")
{
    Mat3 mat(Math::Epsilon, Math::Epsilon, Math::Epsilon,
             Math::Epsilon, Math::Epsilon, Math::Epsilon,
             Math::Epsilon, Math::Epsilon, Math::Epsilon);

    mat.Clean();

    REQUIRE(mat(0, 0) == 0.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 0.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 0.0f);
}

TEST_CASE("Mat3: Translate Function")
{
    Mat3 mat;

    mat.Translate(Vec2(1.0f, 2.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: Rotation Function")
{
    Mat3 mat;

    mat.Rotation(Math::PI / 4.0f);

    REQUIRE(mat(0, 0) == 0.707106781f); REQUIRE(mat(0, 1) == -0.707106781f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.707106781f); REQUIRE(mat(1, 1) == 0.707106781f);  REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f);         REQUIRE(mat(2, 1) == 0.0f);          REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: Euler Rotation Function")
{
    Mat3 mat;

    mat.RotationZXY(Math::PI/4.0f, Math::PI/4.0f, Math::PI/4.0f);

    REQUIRE(Math::IsEqual(mat(0, 0), 0.853553f));  REQUIRE(Math::IsEqual(mat(0, 1), -0.146447f)); REQUIRE(Math::IsEqual(mat(0, 2), 0.5f));
    REQUIRE(Math::IsEqual(mat(1, 0), 0.5f));       REQUIRE(Math::IsEqual(mat(1, 1), 0.5f));       REQUIRE(Math::IsEqual(mat(1, 2), -0.707107f));
    REQUIRE(Math::IsEqual(mat(2, 0), -0.146447f)); REQUIRE(Math::IsEqual(mat(2, 1), 0.853553f));  REQUIRE(Math::IsEqual(mat(2, 2), 0.5f));
}

TEST_CASE("Mat3: Scale Function")
{
    Mat3 mat;

    mat.Scale(Vec2(1.0f, 2.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f);

    mat.Scale(Vec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f);
}

TEST_CASE("Mat3: TRS Function")
{
    Mat3 mat;

    mat.TRS(Vec2(1.0f, 2.0f), Math::PI / 4.0f, Vec2(1.0f, 2.0f));

    REQUIRE(Math::IsEqual(mat(0, 0), sqrt(2.0f) / 2.0f)); REQUIRE(Math::IsEqual(mat(0, 1), -sqrt(2.0f))); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(Math::IsEqual(mat(1, 0), sqrt(2.0f) / 2.0f)); REQUIRE(Math::IsEqual(mat(1, 1), sqrt(2.0f)));  REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f);                           REQUIRE(mat(2, 1) == 0.0f);                     REQUIRE(mat(2, 2) == 1.0f);
}

TEST_CASE("Mat3: Transpose Function")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    const Mat3 const_mat(mat);
    Mat3 result(1.0f, 4.0f, 7.0f,
                2.0f, 5.0f, 8.0f,
                3.0f, 6.0f, 9.0f);

    mat.Transpose();
    Mat3 answer = const_mat.Transpose();

    REQUIRE(mat == result);
    REQUIRE(answer == result);
}

TEST_CASE("Mat3: Inverse Function")
{
    Mat3 mat(10.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    const Mat3 const_mat(mat);
    Mat3 answer(1.0f/9.0f, -2.0f/9.0f, 1.0f/9.0f,
                -2.0f/9.0f, -23.0f/9.0f, 16.0f/9.0f,
                1.0f/9.0f, 22.0f/9.0f, -14.0f/9.0f);

    mat.Inverse();
    Mat3 result = const_mat.Inverse();

    REQUIRE(mat == answer);
    REQUIRE(result == answer);
}

TEST_CASE("Mat3: Determinant Function")
{
    Mat3 mat(10.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);

    float answer = -27.0f;
    float result = mat.Determinant();

    REQUIRE(answer == result);
}

TEST_CASE("Mat3: Set Rows Function")
{
    Mat3 mat;

    mat.SetRows(Vec3(1.0f), Vec3(2.0f), Vec3(3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 1.0f); REQUIRE(mat(0, 2) == 1.0f);
    REQUIRE(mat(1, 0) == 2.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 2.0f);
    REQUIRE(mat(2, 0) == 3.0f); REQUIRE(mat(2, 1) == 3.0f); REQUIRE(mat(2, 2) == 3.0f);
}

TEST_CASE("Mat3: Get Rows Function")
{
    Mat3 mat(1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f);

    Vec3 r0, r1, r2;

    mat.GetRows(r0, r1, r2);

    REQUIRE(r0 == Vec3(1.0f));
    REQUIRE(r1 == Vec3(1.0f));
    REQUIRE(r2 == Vec3(1.0f));
}

TEST_CASE("Mat3: Set Row Function")
{
    Mat3 mat;

    mat.SetRow(0, Vec3(1.0f));
    mat.SetRow(1, Vec3(2.0f));
    mat.SetRow(2, Vec3(3.0f));

    Mat3 answer;
    answer.SetRows(Vec3(1.0f), Vec3(2.0f), Vec3(3.0f));

    REQUIRE(answer == mat);
}

TEST_CASE("Mat3: Get Row Function")
{
    Mat3 mat;

    mat.SetRows(Vec3(1.0f), Vec3(2.0f), Vec3(3.0f));

    auto r0 = mat.GetRow(0);
    auto r1 = mat.GetRow(1);
    auto r2 = mat.GetRow(2);

    REQUIRE(r0 == Vec3(1.0f));
    REQUIRE(r1 == Vec3(2.0f));
    REQUIRE(r2 == Vec3(3.0f));
}

TEST_CASE("Mat3: Set Columns Function")
{
    Mat3 mat;

    mat.SetColumns(Vec3(1.0f), Vec3(2.0f), Vec3(3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 2.0f); REQUIRE(mat(0, 2) == 3.0f);
    REQUIRE(mat(1, 0) == 1.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 3.0f);
    REQUIRE(mat(2, 0) == 1.0f); REQUIRE(mat(2, 1) == 2.0f); REQUIRE(mat(2, 2) == 3.0f);
}

TEST_CASE("Mat3: Get Columns Function")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             1.0f, 2.0f, 3.0f,
             1.0f, 2.0f, 3.0f);

    Vec3 c0, c1, c2;

    mat.GetColumns(c0, c1, c2);

    REQUIRE(c0 == Vec3(1.0f));
    REQUIRE(c1 == Vec3(2.0f));
    REQUIRE(c2 == Vec3(3.0f));
}

TEST_CASE("Mat3: Set Column Function")
{
    Mat3 mat;

    mat.SetColumn(0, Vec3(1.0f));
    mat.SetColumn(1, Vec3(2.0f));
    mat.SetColumn(2, Vec3(3.0f));

    Vec3 c0, c1, c2;

    mat.GetColumns(c0, c1, c2);

    REQUIRE(c0 == Vec3(1.0f));
    REQUIRE(c1 == Vec3(2.0f));
    REQUIRE(c2 == Vec3(3.0f));
}

TEST_CASE("Mat3: Get Column Function")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             1.0f, 2.0f, 3.0f,
             1.0f, 2.0f, 3.0f);

    Vec3 c0, c1, c2;

    c0 = mat.GetColumn(0);
    c1 = mat.GetColumn(1);
    c2 = mat.GetColumn(2);

    REQUIRE(c0 == Vec3(1.0f));
    REQUIRE(c1 == Vec3(2.0f));
    REQUIRE(c2 == Vec3(3.0f));
}

TEST_CASE("Mat3: Transform Vector Function")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             0.0f, 0.0f, 1.0f);
    Vec2 vec(1.0f, 2.0f);
    Vec2 answer(5.0f, 14.0f);

    Vec2 result = mat.TransformVector(vec);
    mat.TransformVectorInplace(vec);

    REQUIRE(result == answer);
    REQUIRE(vec == answer);
}

TEST_CASE("Mat3: Transform Point Function")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             0.0f, 0.0f, 1.0f);
    Vec2 point(1.0f, 2.0f);
    Vec2 answer(8.0f, 20.0f);

    Vec2 result = mat.TransformPoint(point);
    mat.TransformPointInplace(point);

    REQUIRE(result == answer);
    REQUIRE(point == answer);
}

TEST_CASE("Mat3: Transform Point And Divide Function")
{
    Mat3 mat(1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f);
    Vec2 point(1.0f, 2.0f);
    Vec2 answer(1.0f/4.0f, 5.0f/8.0f);

    Vec2 result = mat.TransformPointAndDiv(point);
    mat.TransformPointAndDivInplace(point);

    REQUIRE(result == answer);
    REQUIRE(point == answer);
}

TEST_CASE("Mat4: Clean Function")
{
    Mat4 mat(Math::Epsilon, Math::Epsilon, Math::Epsilon, Math::Epsilon,
             Math::Epsilon, Math::Epsilon, Math::Epsilon, Math::Epsilon,
             Math::Epsilon, Math::Epsilon, Math::Epsilon, Math::Epsilon,
             Math::Epsilon, Math::Epsilon, Math::Epsilon, Math::Epsilon);

    mat.Clean();

    REQUIRE(mat(0, 0) == 0.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 0.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 0.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 0.0f);
}

TEST_CASE("Mat4: Translation Function")
{
    Mat4 mat;

    mat.Translate(Vec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 1.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 1.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 2.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 1.0f); REQUIRE(mat(2, 3) == 3.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("Mat4: Euler Rotation Function")
{
    Mat4 mat;

    mat.RotationZXY(Math::PI/4.0f, Math::PI/4.0f, Math::PI/4.0f);

    REQUIRE(Math::IsEqual(mat(0, 0), 0.853553f));  REQUIRE(Math::IsEqual(mat(0, 1), -0.146447f)); REQUIRE(Math::IsEqual(mat(0, 2), 0.5f));       REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(Math::IsEqual(mat(1, 0), 0.5f));       REQUIRE(Math::IsEqual(mat(1, 1), 0.5f));       REQUIRE(Math::IsEqual(mat(1, 2), -0.707107f)); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(Math::IsEqual(mat(2, 0), -0.146447f)); REQUIRE(Math::IsEqual(mat(2, 1), 0.853553f));  REQUIRE(Math::IsEqual(mat(2, 2), 0.5f));       REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);
}

TEST_CASE("Mat4: Scalar Function")
{
    Mat4 mat;

    mat.Scale(Vec3(1.0f, 2.0f, 3.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 1.0f);

    mat.Scale(Vec4(1.0f, 2.0f, 3.0f, 4.0f));

    REQUIRE(mat(0, 0) == 1.0f); REQUIRE(mat(0, 1) == 0.0f); REQUIRE(mat(0, 2) == 0.0f); REQUIRE(mat(0, 3) == 0.0f);
    REQUIRE(mat(1, 0) == 0.0f); REQUIRE(mat(1, 1) == 2.0f); REQUIRE(mat(1, 2) == 0.0f); REQUIRE(mat(1, 3) == 0.0f);
    REQUIRE(mat(2, 0) == 0.0f); REQUIRE(mat(2, 1) == 0.0f); REQUIRE(mat(2, 2) == 3.0f); REQUIRE(mat(2, 3) == 0.0f);
    REQUIRE(mat(3, 0) == 0.0f); REQUIRE(mat(3, 1) == 0.0f); REQUIRE(mat(3, 2) == 0.0f); REQUIRE(mat(3, 3) == 4.0f);
}

TEST_CASE("Mat4: Tranpose Functions")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    const Mat4 const_mat(mat);
    Mat4 answer(1.0f, 5.0f, 9.0f, 13.0f,
                2.0f, 6.0f, 10.0f, 14.0f,
                3.0f, 7.0f, 11.0f, 15.0f,
                4.0f, 8.0f, 12.0f, 16.0f);

    mat.Transpose();
    Mat4 result = const_mat.Transpose();

    REQUIRE(mat == answer);
    REQUIRE(result == answer);
}

TEST_CASE("Mat4: Inverse Functions")
{
    Mat4 mat(10.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 60.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    const Mat4 const_mat(mat);
    Mat4 answer(1.0f/9.0f, 0.0f, -1.0f/3.0f, 2.0f/9.0f,
                0.0f, 1.0f/54.0f, -1.0f/27.0f, 1.0f/54.0f,
                -1.0f/3.0f, -1.0f/27.0f, -79.0f/27.0f, 62.0f/27.0f,
                2.0f/9.0f, 1.0f/54.0f, 329.0f/108.0f, -247.0f/108.0f);

    mat.Inverse();
    Mat4 result = const_mat.Inverse();

    REQUIRE(answer == mat);
    REQUIRE(answer == result);
}

TEST_CASE("Mat4: Inverse Fast Function")
{
    /*Mat4 foo = Mat4(Vec3(1.0f, 2.0f, 3.0f)) * Mat4(1.0f, 2.0f, 3.0f);
    Mat4 bar;
    bar.RotationZXY(Math::PI/4.0f, Math::PI/4.0f, Math::PI/4.0f);
    Mat4 mat = foo * bar;
    const Mat4 const_mat(mat);
    Mat4 answer(0.853553f, 0.25f, -0.048816f, -1.20711f,
                -0.146447f, 0.25f, 0.284518f, -1.20711f,
                0.5f, -0.353553f, 0.166667f, -0.292893f,
                0.0f, 0.0f, 0.0f, 1.0f);

    mat.InverseFast();
    Mat4 result = const_mat.InverseFast();

    REQUIRE(mat == answer);
    REQUIRE(result == answer);*/
}

TEST_CASE("Mat4: Determinant Function")
{
    Mat4 mat(10.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 60.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);

    float det = mat.Determinant();

    REQUIRE(det == -1944.0f);
}

TEST_CASE("Mat4: Set Rows Function")
{
    Mat4 mat;

    mat.SetRows(Vec4(1.0f), Vec4(2.0f), Vec4(3.0f), Vec4(4.0f));

    Mat4 result(Vec4(1.0f), Vec4(2.0f), Vec4(3.0f), Vec4(4.0f));

    REQUIRE(mat == result);
}

TEST_CASE("Mat4: Get Rows Function")
{
    Mat4 mat(Vec4(1.0f), Vec4(2.0f), Vec4(3.0f), Vec4(4.0f));

    Vec4 r0, r1, r2, r3;

    mat.GetRows(r0, r1, r2, r3);

    REQUIRE(r0 == Vec4(1.0f));
    REQUIRE(r1 == Vec4(2.0f));
    REQUIRE(r2 == Vec4(3.0f));
    REQUIRE(r3 == Vec4(4.0f));
}

TEST_CASE("Mat4: Set Row Function")
{
    Mat4 mat;

    mat.SetRow(0, Vec4(1.0f));
    mat.SetRow(1, Vec4(2.0f));
    mat.SetRow(2, Vec4(3.0f));
    mat.SetRow(3, Vec4(4.0f));

    Vec4 r0, r1, r2, r3;

    mat.GetRows(r0, r1, r2, r3);

    REQUIRE(r0 == Vec4(1.0f));
    REQUIRE(r1 == Vec4(2.0f));
    REQUIRE(r2 == Vec4(3.0f));
    REQUIRE(r3 == Vec4(4.0f));
}

TEST_CASE("Mat4: Get Row Function")
{
    Mat4 mat(Vec4(1.0f), Vec4(2.0f), Vec4(3.0f), Vec4(4.0f));

    Vec4 r0, r1, r2, r3;

    r0 = mat.GetRow(0);
    r1 = mat.GetRow(1);
    r2 = mat.GetRow(2);
    r3 = mat.GetRow(3);

    REQUIRE(r0 == Vec4(1.0f));
    REQUIRE(r1 == Vec4(2.0f));
    REQUIRE(r2 == Vec4(3.0f));
    REQUIRE(r3 == Vec4(4.0f));
}

TEST_CASE("Mat4: Set Columns Function")
{
    Mat4 mat;

    mat.SetColumns(Vec4(1.0f), Vec4(2.0f), Vec4(3.0f), Vec4(4.0f));

    Mat4 result(1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f);

    REQUIRE(mat == result);
}

TEST_CASE("Mat4: Get Columns Function")
{
    Mat4 mat;

    mat.SetColumns(Vec4(1.0f), Vec4(2.0f), Vec4(3.0f), Vec4(4.0f));

    Vec4 c0, c1, c2, c3;

    mat.GetColumns(c0, c1, c2, c3);

    REQUIRE(c0 == Vec4(1.0f));
    REQUIRE(c1 == Vec4(2.0f));
    REQUIRE(c2 == Vec4(3.0f));
    REQUIRE(c3 == Vec4(4.0f));
}

TEST_CASE("Mat4: Set Column Function")
{
    Mat4 mat;

    mat.SetColumn(0, Vec4(1.0f));
    mat.SetColumn(1, Vec4(2.0f));
    mat.SetColumn(2, Vec4(3.0f));
    mat.SetColumn(3, Vec4(4.0f));

    Mat4 result(1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f,
                1.0f, 2.0f, 3.0f, 4.0f);

    REQUIRE(mat == result);
}

TEST_CASE("Mat4: Get Column Function")
{
    Mat4 mat;

    mat.SetColumns(Vec4(1.0f), Vec4(2.0f), Vec4(3.0f), Vec4(4.0f));

    Vec4 c0, c1, c2, c3;

    c0 = mat.GetColumn(0);
    c1 = mat.GetColumn(1);
    c2 = mat.GetColumn(2);
    c3 = mat.GetColumn(3);

    REQUIRE(c0 == Vec4(1.0f));
    REQUIRE(c1 == Vec4(2.0f));
    REQUIRE(c2 == Vec4(3.0f));
    REQUIRE(c3 == Vec4(4.0f));
}

TEST_CASE("Mat4: Transform Vector Function")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             0.0f, 0.0f, 0.0f, 1.0f);
    Vec3 vec(1.0f, 2.0f, 3.0f);
    Vec3 answer(14.0f, 38.0f, 62.0f);

    Vec3 result = mat.TransformVector(vec);
    mat.TransformVectorInplace(vec);

    REQUIRE(result == answer);
    REQUIRE(vec == answer);
}

TEST_CASE("Mat4: Transform Point Function")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             0.0f, 0.0f, 0.0f, 1.0f);
    Vec3 point(1.0f, 2.0f, 3.0f);
    Vec3 answer(18.0f, 46.0f, 74.0f);

    Vec3 result = mat.TransformPoint(point);
    mat.TransformPointInplace(point);

    REQUIRE(result == answer);
    REQUIRE(point == answer);
}

TEST_CASE("Mat4: Transform Point And Divide Function")
{
    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    Vec3 point(1.0f, 2.0f, 3.0f);
    Vec3 answer(3.0f/17.0f, 23.0f/51.0f, 37.0f/51.0f);

    Vec3 result = mat.TransformPointAndDiv(point);
    mat.TransformPointAndDivInplace(point);

    REQUIRE(result == answer);
    REQUIRE(point == answer);
}

/////////////////////////////////////////////////////////
// !Public Method Tests
/////////////////////////////////////////////////////////