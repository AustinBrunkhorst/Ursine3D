#define CATCH_CONFIG_MAIN
#include "../dep/Testing/include/catch.hpp"

#include <SQuat.h>
#include <SVec3.h>
#include <SMat3.h>

using namespace ursine;

static bool LossyEulerEquals(const SVec3 &a, const SVec3 &b)
{
	// This code handles the case where an angle is the exact opposit
	// of the other.  Ex: (-30, 30, 20) == (30, -30, -20)
	auto vec1 = a.X() < 0.0f && b.X() > 0.0f ? -a : a;
	auto vec2 = b.X() < 0.0f && a.X() > 0.0f ? -b : b;

	auto dif = vec1 - vec2;
	return abs(dif.X()) <= 10.0f &&
		abs(dif.Y()) <= 10.0f &&
		abs(dif.Z()) <= 10.0f;
}

static bool LossyVecEquals(const SVec3 &a, const SVec3 &b)
{
	auto dif = a - b;

	return abs(dif.X()) <= 0.1f &&
		   abs(dif.Y()) <= 0.1f &&
		   abs(dif.Z()) <= 0.1f;
}

static bool LossyMatEquals(const SMat3 &a, const SMat3 &b)
{
	SVec3 a0, a1, a2, b0, b1, b2;

	a.GetRows(a0, a1, a2);
	b.GetRows(b0, b1, b2);

	return LossyVecEquals(a0, b0) && LossyVecEquals(a1, b1) && LossyVecEquals(a2, b2);
}

TEST_CASE("SQuat: Equality Operator")
{
	SQuat quat0( 1.0f, 2.0f, 3.0f, 4.0f );
	SQuat quat1( 1.0f, 2.0f, 3.0f, 4.0f );
	SQuat quat2( 0.0f, 2.0f, 3.0f, 4.0f );
	SQuat quat3( 1.0f, 0.0f, 3.0f, 4.0f );
	SQuat quat4( 1.0f, 2.0f, 0.0f, 4.0f );
	SQuat quat5( 1.0f, 2.0f, 3.0f, 0.0f );

	REQUIRE( quat0 == quat1 );
	REQUIRE( quat0 != quat2 );
	REQUIRE( quat0 != quat3 );
	REQUIRE( quat0 != quat4 );
	REQUIRE( quat0 != quat5 );
}

TEST_CASE("SQuat: Quaternion Multiplication")
{
	SQuat one(0.f, 0.f, 0.f, 1.f);
	SQuat i(1.f, 0.f, 0.f, 0.f);
	SQuat j(0.f, 1.f, 0.f, 0.f);
	SQuat k(0.f, 0.f, 1.f, 0.f);

	REQUIRE((i * i) == -one);
	REQUIRE((j * j) == -one);
	REQUIRE((k * k) == -one);
	REQUIRE((i * j * k) == -one);

	REQUIRE((i * j) == k);
	REQUIRE((j * i) == -k);
	REQUIRE((j * k) == i);
	REQUIRE((k * j) == -i);
	REQUIRE((k * i) == j);
	REQUIRE((i * k) == -j);
}

TEST_CASE("SQuat: Quaternion Euler Angle")
{
	SQuat quat0(20.0f, 3.0f, 4.0f);
	SQuat quat1(50.0f, 69.0f, -183.0f);
	SQuat quat2(12.123f, 47.6565f, 4.124f);

	REQUIRE(quat0 == SVec4(0.1743826f, 0.0197054f, 0.0298147f, 0.9840292f));
	REQUIRE(quat1 == SVec4(-0.5222797f, 0.3347338f, -0.7403899f, -0.2588435f));
	REQUIRE(quat2 == SVec4(0.1109873f, 0.398002f, -0.009902827f, 0.9105922f));

	REQUIRE(LossyEulerEquals(quat0.GetEulerAngles(), SVec3(20.0f, 3.0f, 4.0f)));
	REQUIRE(LossyEulerEquals(quat1.GetEulerAngles(), SVec3(50.0f, 69.0f, -183.0f)));
	REQUIRE(LossyEulerEquals(quat2.GetEulerAngles(), SVec3(12.123f, 47.6565f, 4.124f)));
}

TEST_CASE("SQuat: Angle Axis")
{
	SQuat quat0(34.5f, SVec3(1.5f, 2.0f, 2.4f));

	REQUIRE(quat0 == SVec4(0.1283528f, 0.1711371f, 0.2053645f, 0.95502f));

	REQUIRE(LossyVecEquals(quat0.GetAxis(), SVec3(0.4f, 0.6f, 0.7f)));
	REQUIRE(math::IsEqual(quat0.GetAngle(), 34.5f));
}

TEST_CASE("SQuat: From To Functions")
{
	SQuat quat0(SVec3(2.0f, -34.0f, 1.3f), SVec3(-3.0f, -2.4f, 12.0f));

	REQUIRE(quat0 == SQuat(-0.6054515f, -0.04172124f, -0.1597071f, 0.7785767f));
}

TEST_CASE("SQuat: Matrix <-> Quaternion Conversions")
{
	SMat3 oldMat(1.5f, 3.0f, 4.0f);
	SQuat quat(oldMat);
	SMat3 newMat(quat);

	REQUIRE(LossyMatEquals(oldMat, newMat));

	SQuat oldQuat(32.0f, SVec3(1.0f, 3.0f, 4.0f));
	SMat3 mat(oldQuat);
	SQuat newQuat(mat);

	REQUIRE(oldQuat == newQuat);
}

TEST_CASE("SQuat: Get Angle Between Quats")
{
	SQuat quat0(10.0f, 3.0f, -45.0f);
	SQuat quat1(-10.0f, 124.0f, 2.0f);

	REQUIRE(math::IsEqual(quat0.GetAngle(quat1), 127.189f));
}

TEST_CASE("SQuat: Get Inverse")
{
	SQuat quat0(10.0f, 3.0f, -45.0f);

	REQUIRE(quat0.GetInverse() == SQuat(-0.07051444f, -0.05743395f, 0.3832044f, 0.9191754f));
}

TEST_CASE("SQuat: Slerp")
{
	SQuat quat0(10.0f, 3.0f, -45.0f);
	SQuat quat1(100.0f, 23.4f, -2.0f);

	SQuat slerp[] = {
		quat0.Slerp(quat1, 0.443f),
		quat0.Slerp(quat1, 1.0f),
		quat0.Slerp(quat1, 0.1f),
		quat0.Slerp(quat1, 0.24f),
		quat0.Slerp(quat1, 0.12f)
	};

	REQUIRE(slerp[0] == SQuat(0.4035954f, 0.1037648f, -0.3103395f, 0.8544197f));
	REQUIRE(slerp[1] == SQuat(0.747739f, 0.1434208f, -0.1663055f, 0.6266254f));
	REQUIRE(slerp[2] == SQuat(0.1479689f, 0.06872955f, -0.3706284f, 0.9143392f));
	REQUIRE(slerp[3] == SQuat(0.2547285f, 0.08379955f, -0.3491146f, 0.8978919f));
	REQUIRE(slerp[4] == SQuat(0.1633633f, 0.07093887f, -0.3678298f, 0.9126781f));
}

TEST_CASE("SQuat: Rotate Vector")
{
	SQuat quat(34.4f, SVec3(14.3f, -184.4f, 13.9f));
	SVec3 vec(38.3f, 48.23f, -28.0f);

	auto result = quat * vec;
	SVec3 answer(44.6521f, 50.8252f, -0.106816f);

	REQUIRE(LossyVecEquals(result, answer));
}

TEST_CASE("SQuat: Look At")
{
	SQuat quat;

	quat.SetLookAt(
		SVec3(39.0f, 273.0f, -45.0f)
	);

	SQuat answer(-0.2192286f, 0.7297007f, 0.5876916f, 0.2722027f);

	REQUIRE(quat == answer);

	quat.SetLookAt(SVec3(39.0f, 273.0f, -45.0f));

	REQUIRE(quat == answer);
}
