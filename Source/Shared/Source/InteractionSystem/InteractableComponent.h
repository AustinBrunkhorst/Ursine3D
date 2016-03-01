#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractableComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include <BoxColliderComponent.h>

enum class InteractionState;
struct Interaction;
class CommandQueue;

struct Interactable : ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    Interactable(void);

    // Called when object enters collider
    void StartInteraction(const CommandQueue *queue, ursine::ecs::EntityID id);
    
    // called while object's interaction bay has chosen this object as
    //   its current interactable object
    void Interact(const CommandQueue *queue, ursine::ecs::EntityID id);

    // called when object leaves collider
    void StopInteraction(const CommandQueue *queue, ursine::ecs::EntityID id);

    // Interaction objects call this for setting up the interactable for use
    void SetUpInteractable(Interaction *interaction);

private:
    Interaction* m_interaction;
    
    // function pointers for interaction logic
    void (Interaction::*m_startFunc)    (const CommandQueue* queue, ursine::ecs::EntityID id);
    void (Interaction::*m_interactFunc) (const CommandQueue* queue, ursine::ecs::EntityID id);
    void (Interaction::*m_stopFunc)     (const CommandQueue* queue, ursine::ecs::EntityID id);

} Meta(
    Enable,
    WhiteListMethods,
    DisplayName( "Interactable" ), 
    RequiresComponents( typeof( ursine::ecs::BoxCollider ) )
);

