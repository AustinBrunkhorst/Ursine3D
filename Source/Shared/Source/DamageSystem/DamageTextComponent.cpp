/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageTextComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "DamageTextComponent.h"

#include <Scene.h>

NATIVE_COMPONENT_DEFINITION( DamageText ) ;

using namespace ursine;

DamageText::DamageText(void)
    : BaseComponent( ) { }

void DamageText::OnInitialize(void)
{
#if defined(URSINE_WITH_EDITOR)

    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto scene = world->GetOwner( );

    if (!scene)
        return;

    if (scene->GetPlayState( ) == PS_EDITOR)
        return;

#endif

    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &DamageText::onAnimationFinish );
}

void DamageText::onAnimationFinish(EVENT_HANDLER(EntityAnimator))
{
    // Delete the damage text
    GetOwner( )->GetRoot( )->Delete( );
}
