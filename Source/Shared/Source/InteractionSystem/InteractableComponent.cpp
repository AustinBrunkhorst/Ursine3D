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
{
}

void Interactable::StartInteraction(const CommandQueue *queue, ecs::EntityID id)
{
    (m_interaction->*m_startFunc)( queue, id );
}

void Interactable::Interact(const CommandQueue *queue, ecs::EntityID id)
{
    (m_interaction->*m_interactFunc)( queue, id );
}

void Interactable::StopInteraction(const CommandQueue *queue, ecs::EntityID id)
{
    (m_interaction->*m_stopFunc)( queue, id );
}

void Interactable::SetUpInteractable(Interaction *interaction)
{
    m_interaction  = interaction;
    m_startFunc    = &Interaction::StartInteraction;
    m_interactFunc = &Interaction::Interact;
    m_stopFunc     = &Interaction::StopInteraction;
}
