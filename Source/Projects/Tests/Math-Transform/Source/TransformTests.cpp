#define CATCH_CONFIG_RUNNER
#include "../dep/Testing/include/catch.hpp"

#include <EntityManager.h>
#include <Application.h>
#include <World.h>
#include <TransformComponent.h>
#include <SVec3.h>

using namespace ursine;
using namespace ecs;

int main(int argc, char * argv[])
{
    Application application(argc, argv);

    int result = Catch::Session().run(argc, argv);

    return result;
}

bool lossyEquals(const SVec3 &a, const SVec3 &b)
{
    auto dif = a - b;

    dif.X() = abs(dif.X());
    dif.Y() = abs(dif.Y());
    dif.Z() = abs(dif.Z());

    SVec3 ep(0.1f);

    return dif.X() < ep.X() && dif.Y() < ep.Y() && dif.Z() < ep.Z();
}

bool lossyEquals(const SVec4 &a, const SVec4 &b)
{
    auto dif = a - b;

    dif.X() = abs(dif.X());
    dif.Y() = abs(dif.Y());
    dif.Z() = abs(dif.Z());
    dif.W() = abs(dif.W());

    SVec4 ep(0.1f);

    return dif.X() < ep.X() && dif.Y() < ep.Y() && dif.Z() < ep.Z() && dif.W() < ep.W();
}

TEST_CASE("Transform: Manipulation")
{
    World world;

    auto e = world.CreateEntity();
    auto t0 = e->GetComponent<Transform>();
    
    //////////////////////
    // Testing position //
    //////////////////////

    SVec3 parentPos( -20, 25, 329 );
    t0->SetWorldPosition( parentPos );

    // initial testing of values
    REQUIRE( lossyEquals( t0->GetLocalPosition( ), parentPos ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), parentPos ) );

    // create a new entity and parent it to the transform
    auto t1 = world.CreateEntity( )->GetComponent<Transform>( );

    t1->SetWorldPosition( SVec3( 51, 394, 3823 ) );
    t0->AddChild( t1 );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 71, 369, 3494 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 51, 394, 3823 ) ) );

    auto t2 = world.CreateEntity( )->GetComponent<Transform>( );
    
    t2->SetWorldPosition( SVec3( -2, 23, 39 ) );
    t1->AddChild( t2 );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -53, -371, -3784 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( -2, 23, 39 ) ) );

    // Changing the root, effecting all children
    t0->SetWorldPosition( SVec3( 10, 30, 43 ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 71, 369, 3494 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 81, 399, 3537 ) ) );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -53, -371, -3784 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( 28, 28, -247 ) ) );

    // Changing first child, effecting only second child
    t1->SetWorldPosition( SVec3( 100, -30, 15 ) );

    REQUIRE(lossyEquals(t0->GetLocalPosition(), SVec3(10, 30, 43)));
    REQUIRE(lossyEquals(t0->GetWorldPosition(), SVec3(10, 30, 43)));

    REQUIRE(lossyEquals(t2->GetLocalPosition(), SVec3(-53, -371, -3784)));
    REQUIRE(lossyEquals(t2->GetWorldPosition(), SVec3(47, -401, -3769)));

    //////////////////////
    // Testing rotation //
    //////////////////////

    // rotating the first child
    t1->SetLocalRotation( SQuat( 0.0f, 24.0f, 0.0f ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 10, 30, 43 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 10, 30, 43 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat( 0.0f, 0.0f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat( 0.0f, 0.0f, 0.0f, 1.0f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 90, -60, -28 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 100, -30, 15 ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat( 0.0f, 0.2f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat( 0.0f, 0.2f, 0.0f, 1.0f ) ) );


    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -53, -371, -3784 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( -1487.5f, -401, -3420.3f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat( 0.0f, 0.0f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat( 0.0f, 0.2f, 0.0f, 1.0f ) ) );

    // rotating the root
    t0->SetLocalRotation( SQuat( 25.0f, 0.0f, 2.0f ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 10, 30, 43 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 10, 30, 43 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat( 0.2f, 0.0f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat( 0.2f, 0.0f, 0.0f, 1.0f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 90, -60, -28 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 102, -9.7f, -6.4f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat( 0.0f, 0.2f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat( 0.2f, 0.2f, 0.1f, 1.0f ) ) );


    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -53, -371, -3784 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( -1471.6f, 1055.9f, -3299.9f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat( 0.0f, 0.0f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat( 0.2f, 0.2f, 0.1f, 1.0f ) ) );

    // rotating the second child (world coordinates)
    t1->SetWorldRotation( SQuat( 343.0f, 3.0f, -12.0f ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 10, 30, 43 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 10, 30, 43 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat( 0.2f, 0.0f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat( 0.2f, 0.0f, 0.0f, 1.0f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 90, -60, -28 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 102, -9.7f, -6.4f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat( 0.4f, 0.0f, 0.1f, -0.9f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat( 0.1f, 0.0f, 0.1f, -1.0f ) ) );


    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -53, -371, -3784 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( -210.8f, -1452.5f, -3510.6f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat( 0.0f, 0.0f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat( 0.1f, 0.0f, 0.1f, -1.0f ) ) );

    // rotating the leaf
    t2->SetWorldRotation( SQuat( 29.0f, 282.0f, 183.0f ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 10, 30, 43 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 10, 30, 43 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat( 0.2f, 0.0f, 0.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat( 0.2f, 0.0f, 0.0f, 1.0f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 90, -60, -28 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 102, -9.7f, -6.4f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat( 0.4f, 0.0f, 0.1f, -0.9f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat( 0.1f, 0.0f, 0.1f, -1.0f ) ) );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -53, -371, -3784 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( -210.8f, -1452.5f, -3510.6f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat( -0.6f, -0.3f, 0.7f, -0.2f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat( 0.6f, 0.2f, -0.7f, 0.2f ) ) );


}
