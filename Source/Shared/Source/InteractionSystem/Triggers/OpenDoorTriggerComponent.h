#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

struct OpenDoorTrigger 
    : ursine::ecs::Component
    , Interaction
{
    NATIVE_COMPONENT;

public:
    OpenDoorTrigger(void);
    ~OpenDoorTrigger(void);

protected:
    void OnInitialize(void) override;

private:
    // interaction logic
    void StartInteraction(const ursine::ecs::EntityHandle &entity) override;
    void Interact(const ursine::ecs::EntityHandle &entity) override;
    void StopInteraction(const ursine::ecs::EntityHandle &entity) override;

    void OnAreaClear(EVENT_HANDLER(game::AREA_CLEAR));

    bool m_clear;

} Meta(Enable, DisplayName("OpenDoorTrigger"));

