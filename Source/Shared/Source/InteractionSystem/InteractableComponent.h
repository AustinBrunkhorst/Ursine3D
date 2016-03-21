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
    Meta(Enable)
    Interactable(void);

    // Called when object enters collider
    void StartInteraction(ursine::ecs::EntityHandle &entity);
    
    // called while object's interaction bay has chosen this object as
    //   its current interactable object
    void Interact(ursine::ecs::EntityHandle &entity);

    // called when object leaves collider
    void StopInteraction(ursine::ecs::EntityHandle &entity);

    // Interaction objects call this for setting up the interactable for use
    void SetUpInteractable(Interaction *interaction);

private:
    Interaction* m_interaction;
    
    // function pointers for interaction logic
    void (Interaction::*m_startFunc)    (ursine::ecs::EntityHandle &entity);
    void (Interaction::*m_interactFunc) (ursine::ecs::EntityHandle &entity);
    void (Interaction::*m_stopFunc)     (ursine::ecs::EntityHandle &entity);

} Meta(
    Enable,
    WhiteListMethods,
    DisplayName( "Interactable" ), 
    RequiresComponents( typeof( ursine::ecs::BoxCollider ) )
);

