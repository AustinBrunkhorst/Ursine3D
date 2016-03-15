/*---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FaceBoneTowardsOrigin.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "FaceBoneTowardsOrigin.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>

NATIVE_COMPONENT_DEFINITION(FaceBoneTowardsOrigin);

using namespace ursine;
using namespace ecs;

FaceBoneTowardsOrigin::FaceBoneTowardsOrigin(void)
    : BaseComponent( )
{
    
}

void FaceBoneTowardsOrigin::OnSceneReady(Scene *scene)
{
    auto animator = GetOwner( )->GetComponentsInParents<Animator>( );
    
    if (animator.size( ))
        animator[0]->GetOwner( )->Listener( this )
            .On( ENTITY_ANIMATION_BONE_MANIPULATION_VALID, &FaceBoneTowardsOrigin::onAnimationManipulation );
}

void FaceBoneTowardsOrigin::onAnimationManipulation(EVENT_HANDLER(Entity))
{
    auto trans = GetOwner( )->GetTransform( );

    auto pos = trans->GetWorldPosition( );

    trans->SetWorldRotation( SQuat::LookAt( pos ) );
}
