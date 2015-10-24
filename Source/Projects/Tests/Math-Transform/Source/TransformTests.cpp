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
    Application application( argc, argv );

    int result = Catch::Session( ).run( argc, argv );

    return result;
}

bool lossyEquals(const SVec3 &a, const SVec3 &b, float loss = 0.1f)
{
    auto dif = a - b;

    dif.X( ) = abs( dif.X( ) );
    dif.Y( ) = abs( dif.Y( ) );
    dif.Z( ) = abs( dif.Z( ) );

    SVec3 ep( loss );

    return dif.X( ) < ep.X( ) && 
           dif.Y( ) < ep.Y( ) && 
           dif.Z( ) < ep.Z( );
}

bool lossyEquals(const SVec4 &a, const SVec4 &b, float loss = 0.1f)
{
    auto dif = a - b;

    dif.X( ) = abs( dif.X( ) );
    dif.Y( ) = abs( dif.Y( ) );
    dif.Z( ) = abs( dif.Z( ) );
    dif.W( ) = abs( dif.W( ) );

    SVec4 ep( loss );

    return dif.X( ) < ep.X( ) && 
           dif.Y( ) < ep.Y( ) && 
           dif.Z( ) < ep.Z( ) && 
           dif.W( ) < ep.W( );
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

    // testing the look at function for the parent

    // minimize float impercision
    t0->SetWorldPosition( SVec3( 2, 2, -3 ) );
    t1->SetLocalPosition( SVec3( 1, 1, 1 ) );
    t2->SetLocalPosition( SVec3( -2, -3, -4 ) );

    t0->LookAt( SVec3( 50, -5, 32 ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat( 0.1f, 0.5f, 0.0f, 0.9f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat( 0.1f, 0.5f, 0.0f, 0.9f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 1, 1, 1 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 3.5f, 2.9f, -3.2f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat( 0.4f, 0.0f, 0.1f, -0.9f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat( 0.3f, -0.4f, 0.0f, -0.8f ) ) );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -2, -3, -4 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( 0.4f, -1.3f, -1.8f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat( -0.6f, -0.3f, 0.7f, -0.2f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat( 0.2f, 0.2f, -1.0f, 0.2f ) ) );


    t1->LookAt( SVec3( 34, 38, 20 ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat( 0.1f, 0.5f, 0.0f, 0.9f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat( 0.1f, 0.5f, 0.0f, 0.9f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 1, 1, 1 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 3.5f, 2.9f, -3.2f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat( -0.4f, 0.0f, 0.0f, 0.9f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat( -0.3f, 0.4f, 0.2f, 0.8f ) ) );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -2, -3, -4 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( 1.5f, -2.0f, -2.1f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat( -0.6f, -0.3f, 0.7f, -0.2f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat( -0.1f, -0.2f, 0.9f, -0.3f ) ) );

    t2->LookAt( SVec3( 48, -2, 3 ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat( 0.1f, 0.5f, 0.0f, 0.9f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat( 0.1f, 0.5f, 0.0f, 0.9f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 1, 1, 1 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 3.5f, 2.9f, -3.2f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat( -0.4f, 0.0f, 0.0f, 0.9f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat( -0.3f, 0.4f, 0.2f, 0.8f ) ) );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -2, -3, -4 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( 1.5f, -2.0f, -2.1f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat( 0.3f, 0.2f, 0.1f, 0.9f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat( 0.0f, 0.7f, 0.0f, 0.7f ) ) );

    //////////////////////
    // Testing scale    //
    //////////////////////

    t0->SetWorldRotation( SQuat::Identity( ) );
    t1->SetWorldRotation( SQuat::Identity( ) );
    t2->SetWorldRotation( SQuat::Identity( ) );

    t0->SetLocalScale( SVec3( 10, 20, -2 ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t0->GetLocalScale( ), SVec3( 10.0f, 20.0f, -2.0f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldScale( ), SVec3( 10.0f, 20.0f, -2.0f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 1, 1, 1 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 12.0f, 22.0f, -5.0f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t1->GetLocalScale( ), SVec3( 1.0f, 1.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldScale( ), SVec3( 10.0f, 20.0f, -2.0f ) ) );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -2, -3, -4 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( -8.0f, -38.0f, 3.0f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t2->GetLocalScale( ), SVec3( 1.0f, 1.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldScale( ), SVec3( 10.0f, 20.0f, -2.0f ) ) );

    t1->SetLocalScale( SVec3( 75, 92, 1.4f ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t0->GetLocalScale( ), SVec3( 10.0f, 20.0f, -2.0f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldScale( ), SVec3( 10.0f, 20.0f, -2.0f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 1, 1, 1 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 12.0f, 22.0f, -5.0f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t1->GetLocalScale( ), SVec3( 75.0f, 92.0f, 1.4f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldScale( ), SVec3( 750.0f, 1840.0f, -2.8f ) ) );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -2, -3, -4 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( -1488.0f, -5498.0f, 6.2f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t2->GetLocalScale( ), SVec3( 1.0f, 1.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldScale( ), SVec3( 750.0f, 1840.0f, -2.8f ) ) );

    t2->SetLocalScale( SVec3( 1.2f, 0.2f, 1.6f ) );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t0->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t0->GetLocalScale( ), SVec3( 10.0f, 20.0f, -2.0f ) ) );
    REQUIRE( lossyEquals( t0->GetWorldScale( ), SVec3( 10.0f, 20.0f, -2.0f ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 1, 1, 1 ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 12.0f, 22.0f, -5.0f ) ) );
    REQUIRE( lossyEquals( t1->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t1->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t1->GetLocalScale( ), SVec3( 75.0f, 92.0f, 1.4f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldScale( ), SVec3( 750.0f, 1840.0f, -2.8f ) ) );

    REQUIRE( lossyEquals( t2->GetLocalPosition( ), SVec3( -2, -3, -4 ) ) );
    REQUIRE( lossyEquals( t2->GetWorldPosition( ), SVec3( -1488.0f, -5498.0f, 6.2f ) ) );
    REQUIRE( lossyEquals( t2->GetLocalRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t2->GetWorldRotation( ), SQuat::Identity( ) ) );
    REQUIRE( lossyEquals( t2->GetLocalScale( ), SVec3( 1.2f, 0.2f, 1.6f ) ) );
    REQUIRE( lossyEquals( t2->GetWorldScale( ), SVec3( 900.0f, 368.0f, -4.5f ) ) );

    //////////////////////
    // Testing Axis     //
    //////////////////////

    t0->SetWorldScale( SVec3::One( ) );
    t1->SetWorldScale( SVec3::One( ) );
    t2->SetWorldScale( SVec3::One( ) );

    t0->SetWorldRotation( SQuat( 24, 14, 29 ) );
    t1->SetWorldRotation( SQuat( -40, -52, 209 ) );
    t2->SetWorldRotation( SQuat( 240, 67, 15 ) );

    REQUIRE( lossyEquals( t0->GetRight( ), SVec3( 0.9f, 0.4f, 0.0f ) ) );
    REQUIRE( lossyEquals( t0->GetUp( ), SVec3( -0.4f, 0.8f, 0.5f ) ) );
    REQUIRE( lossyEquals( t0->GetForward( ), SVec3( 0.2f, -0.4f, 0.9f ) ) );

    REQUIRE( lossyEquals( t1->GetRight( ), SVec3( -0.8f, -0.4f, -0.5f ) ) );
    REQUIRE( lossyEquals( t1->GetUp( ), SVec3( -0.1f, -0.7f, 0.7f ) ) );
    REQUIRE( lossyEquals( t1->GetForward( ), SVec3( -0.6f, 0.6f, 0.5f ) ) );

    REQUIRE( lossyEquals( t2->GetRight( ), SVec3( 0.2f, -0.1f, -1.0f ) ) );
    REQUIRE( lossyEquals( t2->GetUp( ), SVec3( -0.9f, -0.5f, -0.1f ) ) );
    REQUIRE( lossyEquals( t2->GetForward( ), SVec3( -0.5f, 0.9f, -0.2f ) ) );

    //////////////////////
    // Testing Hierarch //
    //////////////////////

    REQUIRE( t1->IsChildOf( t0 ) );
    REQUIRE( t2->IsChildOf( t0 ) );
    REQUIRE( t2->IsChildOf( t1 ) );
    REQUIRE( !t0->IsChildOf( t0 ) );

    REQUIRE( t0->GetParent( ) == nullptr );
    REQUIRE( t1->GetParent( ) == t0 );
    REQUIRE( t2->GetParent( ) == t1 );

    REQUIRE( t0->GetRoot( ) == t0 );
    REQUIRE( t1->GetRoot( ) == t0 );
    REQUIRE( t2->GetRoot( ) == t0 );

    // this shouldn't do anything
    t1->AddChild( t0 );

    REQUIRE( t0->GetParent( ) == nullptr );
    REQUIRE( t1->GetParent( ) == t0 );
    REQUIRE( t2->GetParent( ) == t1 );

    t0->RemoveChild( t1 );

    REQUIRE( t0->GetParent( ) == nullptr );
    REQUIRE( t1->GetParent( ) == nullptr );
    REQUIRE( t2->GetParent( ) == t1 );
    REQUIRE( t1->GetWorldPosition( ) == t1->GetLocalPosition( ) );

    t1->SetLocalPosition( SVec3( 1, 1, 1 ) );
    t0->AddChildAlreadyInLocal( t1 );

    REQUIRE( lossyEquals( t0->GetLocalPosition( ), SVec3( 2, 2, -3 ) ) );
    REQUIRE( lossyEquals( t0->GetWorldPosition( ), SVec3( 2, 2, -3 ) ) );

    REQUIRE( lossyEquals( t1->GetLocalPosition( ), SVec3( 1.0f, 1.0f, 1.0f ) ) );
    REQUIRE( lossyEquals( t1->GetWorldPosition( ), SVec3( 2.7, 2.8, -1.7 ) ) );

    t1->RemoveChild( t2 );
    t0->AddChild( t2 );

    t2->SetAsFirstSibling( );
    
    REQUIRE( t0->GetChild( 0 ) == t2 );
    REQUIRE( t2->GetSiblingIndex( ) == 0 );
    REQUIRE( t1->GetSiblingIndex( ) == 1 );

    auto transforms = t0->GetComponentsInChildren<Transform>( );

    REQUIRE( transforms[0] == t2 );
    REQUIRE( transforms[1] == t1 );
}
