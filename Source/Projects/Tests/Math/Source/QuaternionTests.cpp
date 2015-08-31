#define CATCH_CONFIG_MAIN
#include "../dep/Testing/include/catch.hpp"

#include "SQuat.h"

using namespace Ursine;

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
	SQuat one(1.f, 0.f, 0.f, 0.f);
	SQuat i(0.f, 1.f, 0.f, 0.f);
	SQuat j(0.f, 0.f, 1.f, 0.f);
	SQuat k(0.f, 0.f, 0.f, 1.f);

	/*SQuat hi = i * i;
	SQuat hii = -one;

	REQUIRE(i * i == -one);
	REQUIRE(j * j == -one);
	REQUIRE(k * k == -one);
	REQUIRE(i * j * k == -one);
	
	REQUIRE(i * j == k);
	REQUIRE(j * i == -k);
	REQUIRE(j * k == i);
	REQUIRE(k * j == -i);
	REQUIRE(k * i == j);
	REQUIRE(i * k == -j);*/
}





/*http://lolengine.net/browser/trunk/test/unit/quat.cpp?rev=3293

http://commons.apache.org/proper/commons-math/xref-test/org/apache/commons/math3/complex/QuaternionTest.html*/

