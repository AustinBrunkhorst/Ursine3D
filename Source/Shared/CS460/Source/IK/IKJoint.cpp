/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** IKJoint.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "IKJoint.h"

using namespace ursine;
using namespace ecs;

IKJoint::IKJoint(void)
    : transform( nullptr ) { }

IKJoint::IKJoint(Transform *transform)
    : transform( transform ) { }

SVec3 IKJoint::GetWorldPosition(void) const
{
    return transform->GetWorldPosition( );
}

void IKJoint::SetWorldPosition(const SVec3& position)
{
    transform->SetWorldPosition( position );
}

void IKJoint::UpdateOrientation(const SVec3 &goalPosition)
{
    worldBindPose = transform->GetWorldRotation( );

    auto &children = transform->GetChildren( );

    if (children.size( ) == 0)
        return;

    childWorldDirection = children[ 0 ]->GetWorldPosition( ) - transform->GetWorldPosition( );
    childWorldDirection.Normalize( );
}

void IKJoint::CorrectOrientation(const SVec3 &goalPosition) const
{
    auto &children = transform->GetChildren( );

    if (children.size( ) == 0)
    {
        transform->SetLocalRotation( SQuat::Identity( ) );
    }
    else
    {
        // get the new world direction
        auto child = children[ 0 ];

        auto oldPosition = child->GetWorldPosition( );
        auto oldRotation = child->GetWorldRotation( );

        auto newDirection = oldPosition - transform->GetWorldPosition( );
        newDirection.Normalize( );

        transform->SetWorldRotation(
            worldBindPose * SQuat( childWorldDirection, newDirection )
        );

        child->SetWorldPosition( oldPosition );
        child->SetWorldRotation( oldRotation );
    }
}
