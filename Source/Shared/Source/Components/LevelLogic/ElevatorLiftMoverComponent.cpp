/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ElevatorLiftMoverComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ElevatorLiftMoverComponent.h"

#include <LocalTweenManager.h>

NATIVE_COMPONENT_DEFINITION( ElevatorLiftMover );

using namespace ursine;
using namespace ecs;

ElevatorLiftMover::ElevatorLiftMover(void)
    : BaseComponent( )
    , m_duration( 1.0f )
{

}

const SVec3& ElevatorLiftMover::GetStartPosition(void) const
{
    return m_startPos;
}

void ElevatorLiftMover::SetStartPosition(const SVec3& startPosition)
{
    m_startPos = startPosition;
}

const SVec3& ElevatorLiftMover::GetEndPosition(void) const
{
    return m_endPos;
}

void ElevatorLiftMover::SetEndPosition(const SVec3& endPosition)
{
    m_endPos = endPosition;
}

float ElevatorLiftMover::GetDuration(void) const
{
    return m_duration;
}

void ElevatorLiftMover::SetDuration(float duration)
{
    m_duration = duration;
}

void ElevatorLiftMover::StartMoving(void)
{
    auto *owner = GetOwner( );
    auto *trans = owner->GetTransform( );
    
    m_tween = owner->GetTweens( ).Create( )
        .Setter( 
            trans, &Transform::SetLocalPosition, m_startPos, m_endPos,
            TimeSpan::FromSeconds( m_duration ), ease::QuadraticInOut
        );
}

#if defined(URSINE_WITH_EDITOR)

void ElevatorLiftMover::reset(void)
{
    GetOwner( )->GetTransform( )->SetLocalPosition( m_startPos );
}

void ElevatorLiftMover::startMoving(void)
{
    StartMoving( );
}

#endif
