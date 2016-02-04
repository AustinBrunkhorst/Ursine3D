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
    typedef std::vector<Handle<Interactable>> InteractVec;
    typedef std::vector<Handle<ursine::ecs::Transform>> TransVec;

    InteractionBay(void);
    ~InteractionBay(void);

    void OnInitialize(void) override;

    void OnCollision(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    void Clear(void);

    InteractionBay::InteractVec m_interactables;
    InteractionBay::TransVec m_transforms;

    ursine::ecs::Component::Handle<Interactable> m_prevInteractable;

} Meta(Enable, DisplayName("InteractionBay"));
