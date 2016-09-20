/*---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FaceBoneTowardsTargetComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "FaceBoneTowardsTargetComponent.h"

#include <AnimatorComponent.h>
#include <SystemManager.h>
#include <EntityEvent.h>
#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( FaceBoneTowardsTarget );

using namespace ursine;
using namespace ecs;

FaceBoneTowardsTarget::FaceBoneTowardsTarget(void)
    : BaseComponent( )
    , m_maxViewAngle( 30.0f )
    , m_localForward( 0.0f, 0.0f, 1.0f )
    , m_subscribed( false ) { }

FaceBoneTowardsTarget::~FaceBoneTowardsTarget(void)
{
    auto animator = GetOwner( )->GetComponentsInParents<Animator>( );
    
    if (animator.size( ))
        animator[ 0 ]->GetOwner( )->Listener( this )
            .Off( ENTITY_ANIMATION_BONE_MANIPULATION_VALID, &FaceBoneTowardsTarget::onAnimationManipulation );

    auto world = GetOwner( )->GetWorld( );

    if (world)
    {
        world->Listener( this )
        #if defined(URSINE_WITH_EDITOR)
            .Off( WORLD_EDITOR_UPDATE, &FaceBoneTowardsTarget::onUpdate )
        #endif
            .Off( WORLD_UPDATE, &FaceBoneTowardsTarget::onUpdate );
    }
}

void FaceBoneTowardsTarget::OnSceneReady(Scene *scene)
{
    auto world = GetOwner( )->GetWorld( );

    world->Listener( this )
    #if defined(URSINE_WITH_EDITOR)
        .On( WORLD_EDITOR_UPDATE, &FaceBoneTowardsTarget::onUpdate )
    #endif
        .On( WORLD_UPDATE, &FaceBoneTowardsTarget::onUpdate );
}

void FaceBoneTowardsTarget::SetTargetPosition(const SVec3 &worldPosition)
{
    m_targetPosition = worldPosition;
}

const SVec3 &FaceBoneTowardsTarget::GetLocalForward(void) const
{
    return m_localForward;
}

void FaceBoneTowardsTarget::SetLocalForward(const SVec3 &forward)
{
    m_localForward = forward;

    m_localForward.Normalize( );

    NOTIFY_COMPONENT_CHANGED( "localForwardVector", m_localForward );
}

float FaceBoneTowardsTarget::GetMaxViewAngle(void) const
{
    return m_maxViewAngle;
}

void FaceBoneTowardsTarget::SetMaxViewAngle(float degrees)
{
    m_maxViewAngle = degrees;

    NOTIFY_COMPONENT_CHANGED( "maxViewAngle", m_maxViewAngle );
}

void FaceBoneTowardsTarget::onUpdate(EVENT_HANDLER(Entity))
{
    if (!m_subscribed)
    {
        auto animator = GetOwner( )->GetComponentsInParents<Animator>( );
    
        if (animator.size( ))
        {
            animator[ 0 ]->GetOwner( )->Listener( this )
                .On( ENTITY_ANIMATION_BONE_MANIPULATION_VALID, &FaceBoneTowardsTarget::onAnimationManipulation );

            m_subscribed = true;
        }
    }
    else
    {
        auto world = GetOwner( )->GetWorld( );

        world->Listener( this )
            .Off( WORLD_UPDATE, &FaceBoneTowardsTarget::onUpdate );
    }
}

void FaceBoneTowardsTarget::onAnimationManipulation(EVENT_HANDLER(Entity))
{
    auto trans = GetOwner( )->GetTransform( );

    // get direction from position to target.
    auto pos = trans->GetWorldPosition( );
    auto dir = SVec3::Normalize( m_targetPosition - pos );

    // Get the angle from our forward vector to the target direction
    auto worldForward = trans->GetWorldRotation( ).Rotate( m_localForward );

    auto rotation = SQuat( worldForward, dir );

    auto angle = abs(rotation.GetAngle( ));

    // If the angle is within our field of view
    if (angle <= m_maxViewAngle)
    {
        // Set our transforms new orientation
        trans->SetWorldRotation( rotation * trans->GetWorldRotation( ) );
    }
    else
    {
        // Get an orientation that's on the cusp of our view cone
        float t = m_maxViewAngle / angle;

        rotation = SQuat( angle * t, SVec3::Cross( worldForward, dir ) );

        // Now rotate and set the transform's world orientation
        trans->SetWorldRotation( rotation * trans->GetWorldRotation( ) );
    }
}

#if defined(URSINE_WITH_EDITOR)

void FaceBoneTowardsTarget::visualizeFOV(void)
{
    static float FocalLength = 25.0f;

    auto world = GetOwner( )->GetWorld( );
    auto drawer = world->GetEntitySystem<DebugSystem>( );

    auto trans = GetOwner( )->GetTransform( );

    // The position of the bone
    auto position = trans->GetWorldPosition( );

    // The position of the center focal point
    auto focus = trans->ToWorld( m_localForward * FocalLength );

    drawer->DrawCone( position, focus, FocalLength, m_maxViewAngle );
}

#endif
