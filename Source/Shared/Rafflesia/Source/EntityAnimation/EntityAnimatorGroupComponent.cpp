/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorGroupComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EntityAnimatorGroupComponent.h"

#include "EntityAnimatorComponent.h"

NATIVE_COMPONENT_DEFINITION( EntityAnimatorGroup );

EntityAnimatorGroup::EntityAnimatorGroup(void)
    : BaseComponent( )
{
}

void EntityAnimatorGroup::PlayGroupAnimators(void)
{
    auto animators = GetOwner( )->GetComponentsInChildren<EntityAnimator>( );

    for (auto &animator : animators)
        animator->Play( );
}

void EntityAnimatorGroup::StopGroupAnimators(void)
{
    auto animators = GetOwner( )->GetComponentsInChildren<EntityAnimator>( );

    for (auto &animator : animators)
        animator->Stop( );
}

#if defined(URSINE_WITH_EDITOR)

void EntityAnimatorGroup::playGroupAnimators(void)
{
    PlayGroupAnimators( );
}

void EntityAnimatorGroup::stopGroupAnimators(void)
{
    StopGroupAnimators( );
}

#endif
