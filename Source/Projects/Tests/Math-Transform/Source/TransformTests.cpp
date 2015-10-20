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

TEST_CASE("Transform: Local/World Position Manipulation")
{
    World world;

    auto e = world.CreateEntity();
    auto t0 = e->GetComponent<Transform>();
    
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
}
