#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WeaponPickup.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include "BaseInteraction.h"
#include <Entity.h>

struct Inventory;

struct OpenDoorTrigger : ursine::ecs::Component, Interaction
{
    NATIVE_COMPONENT
public:

    OpenDoorTrigger(void);
    ~OpenDoorTrigger(void);

private:
    // interaction logic
    void StartInteraction(const CommandQueue* queue, ursine::ecs::EntityUniqueID id) override;
    void Interact(const CommandQueue* queue, ursine::ecs::EntityUniqueID id) override;
    void StopInteraction(const CommandQueue* queue, ursine::ecs::EntityUniqueID id) override;


    void OnInitialize(void) override;

    bool m_active;

} Meta(Enable, DisplayName("OpenDoorTrigger"));

