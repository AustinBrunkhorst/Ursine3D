#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractionBay.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include <vector>


namespace ursine
{
    namespace ecs
    {
        class Transform;
    } // ecs namespace

} // ursine namespace

struct Interactable;

struct InteractionBay : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:
    Meta(Disable)
    typedef std::vector<Interactable*> InteractVec;
    Meta(Disable)
    typedef std::vector<ursine::ecs::Transform*> TransVec;

    Meta(Enable)
    InteractionBay(void);
    ~InteractionBay(void);

    Meta(Enable)
    void OnInitialize(void) override;
    Meta(Enable)
    void OnCollision(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    void Clear(void);

    Meta(Disable)
    InteractionBay::InteractVec m_interactables;
    Meta(Disable)
    InteractionBay::TransVec m_transforms;

    Meta(Disable)
    Interactable* m_prevInteractable;

} Meta(Enable, WhiteListMethods, DisplayName("InteractionBay"));
