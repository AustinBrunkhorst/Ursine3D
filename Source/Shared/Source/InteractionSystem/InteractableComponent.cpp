/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractableComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "InteractableComponent.h"
#include "BaseInteraction.h"

NATIVE_COMPONENT_DEFINITION( Interactable );

using namespace ursine;

Interactable::Interactable(void) 
    : BaseComponent( )
    , m_interaction( nullptr )
    , m_startFunc( nullptr )
    , m_interactFunc( nullptr )
    , m_stopFunc( nullptr )
    , m_type( NOT_SET )
{
}

void Interactable::StartInteraction(const ecs::EntityHandle &entity)
{
    (m_interaction->*m_startFunc)( entity );
}

void Interactable::Interact(const ecs::EntityHandle &entity)
{
    (m_interaction->*m_interactFunc)( entity );
}

void Interactable::StopInteraction(const ecs::EntityHandle &entity)
{
    (m_interaction->*m_stopFunc)( entity );
}

void Interactable::SetUpInteractable(Interaction *interaction, InteractType type)
{
    m_interaction  = interaction;

    m_startFunc    = &Interaction::StartInteraction;
    m_interactFunc = &Interaction::Interact;
    m_stopFunc     = &Interaction::StopInteraction;

    m_type = type;
}

Interactable::InteractType Interactable::GetInteractType(void) const
{
    return m_type;
}
