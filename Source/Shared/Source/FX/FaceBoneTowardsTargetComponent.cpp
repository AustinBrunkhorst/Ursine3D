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
    , m_localForward( 0.0f, 0.0f, 1.0f ) { }

FaceBoneTowardsTarget::~FaceBoneTowardsTarget(void)
{
    auto animator = GetOwner( )->GetComponentsInParents<Animator>( );
    
    if (animator.size( ))
        animator[ 0 ]->GetOwner( )->Listener( this )
            .Off( ENTITY_ANIMATION_BONE_MANIPULATION_VALID, &FaceBoneTowardsTarget::onAnimationManipulation );
}

void FaceBoneTowardsTarget::OnSceneReady(Scene *scene)
{
    auto animator = GetOwner( )->GetComponentsInParents<Animator>( );
    
    if (animator.size( ))
        animator[ 0 ]->GetOwner( )->Listener( this )
            .On( ENTITY_ANIMATION_BONE_MANIPULATION_VALID, &FaceBoneTowardsTarget::onAnimationManipulation );
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

void FaceBoneTowardsTarget::onAnimationManipulation(EVENT_HANDLER(Entity))
{
    auto trans = GetOwner( )->GetTransform( );

    // get direction from position to target.

    // get the angle to the target
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

    // The direction from the base to the focal point
    auto dir = focus - position;

    // The orthogonal vectors
    SVec3 u, v;

    dir.GenerateOrthogonalVectors( u, v );

    // The radius of the focal ring
    float radius = FocalLength * sinf( math::DegreesToRadians( m_maxViewAngle ) );

    if (m_maxViewAngle > 45.0f)
    {
        float angle = m_maxViewAngle - 45.0f;
        float t = angle / 45.0f;

        focus -= dir * t;
    }

    // render the circle
    drawer->DrawCircle( focus, dir, radius, Color::Gold, 5.0f, true );

    // render the lines
    drawer->DrawLine( position, focus + u * radius, Color::Gold, 5.0f, true );
    drawer->DrawLine( position, focus - u * radius, Color::Gold, 5.0f, true );
    drawer->DrawLine( position, focus + v * radius, Color::Gold, 5.0f, true );
    drawer->DrawLine( position, focus - v * radius, Color::Gold, 5.0f, true );
}

#endif
