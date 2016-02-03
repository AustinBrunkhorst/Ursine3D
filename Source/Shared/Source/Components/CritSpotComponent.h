#pragma once
/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CritSpotComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>

struct CritSpot : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    CritSpot(void);
    ~CritSpot(void);

    void OnInitialize(void) override;

    void OnCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

} Meta(Enable, DisplayName("CritSpot"), RequiresComponents(typeof(ursine::ecs::BoxCollider)));