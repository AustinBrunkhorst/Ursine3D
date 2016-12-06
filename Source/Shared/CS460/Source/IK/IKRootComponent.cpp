/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** IKRootComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "IKRootComponent.h"
#include "IKLeafComponent.h"
#include "EntityEvent.h"
#include "AnimatorComponent.h"

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION(IKRoot);

IKRoot::IKRoot(void)
    : BaseComponent( )
    , m_t( 0.0f ) { }

void IKRoot::OnInitialize(void)
{
    // subscribe to the animator's bone manipulation event
    auto animator = GetOwner( )->GetComponent<Animator>( );

    if (animator == nullptr)
    {
        animator = GetOwner( )->GetComponentInParents<Animator>( );
    }

    UWarningIf(animator == nullptr, "Warning: The IKRoot component requires an animator to be in the hierarchy.");

    animator->GetOwner( )->Listener( this ).On(
        ENTITY_ANIMATION_BONE_MANIPULATION_VALID, &IKRoot::onAnimationManipulation
    );

    // subscribe to this entities hierarchy serialization finish
    GetOwner( )->Listener( this ).On(
        ENTITY_HIERARCHY_SERIALIZED, &IKRoot::onHierarchySerialized
    );
}

float IKRoot::GetT(void) const
{
    return m_t;
}

void IKRoot::SetT(float t)
{
    m_t = t;

    NOTIFY_COMPONENT_CHANGED("t", m_t);
}

void IKRoot::onHierarchySerialized(EVENT_HANDLER(Entity))
{
    m_leaves = GetOwner( )->GetComponentsInChildren<IKLeaf>( );

    UWarningIf(m_leaves.size( ) == 0, "Warning: IKRoot is missing valid IKLeaf children.");

    // for each leaf, initialize it with this as it's root
    for (auto &leaf : m_leaves)
        leaf->Initialize( this );

    GetOwner( )->Listener( this ).Off(
        ENTITY_HIERARCHY_SERIALIZED, &IKRoot::onHierarchySerialized
    );
}

void IKRoot::onAnimationManipulation(EVENT_HANDLER(World))
{
    for (auto &leaf : m_leaves)
        leaf->IKUpdate( m_t );
}
