#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseInteraction.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** Description:
** - This is the base funtionality for interaction components.  Interaction
**     components will define what happens during an interaction with another
**     object that has an Interaction Bay component.  
**
** Important:
** - If inheriting from this class, you must connect the abstract functions
**     to the Interactable component of the entity that you are connected to.
**
** -------------------------------------------------------------------------*/

#include "InteractableComponent.h"

namespace ursine
{
    namespace ecs
    {
        class Entity;

    } //ecs namespace

} // ursine namespace

class CommandQueue;

struct Interaction
{
    virtual void StartInteraction(const CommandQueue* queue, ursine::ecs::EntityID id) = 0;
    virtual void Interact(const CommandQueue* queue, ursine::ecs::EntityID id) = 0;
    virtual void StopInteraction(const CommandQueue* queue, ursine::ecs::EntityID id) = 0;
};
