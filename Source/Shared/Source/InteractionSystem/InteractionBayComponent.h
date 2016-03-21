#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractionBay.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include <vector>
#include <queue>

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
private:
    NATIVE_COMPONENT;

    bool Compare(std::pair<float, Interactable*>& lhs, std::pair<float, Interactable*>& rhs)
    {
        return lhs.first < rhs.first;
    }

public:
    Meta(Disable)
    typedef std::vector<Interactable*> InteractVec;
    Meta(Disable)
    typedef std::priority_queue<std::pair<float, Interactable*>,
                                std::vector<std::pair<float, Interactable*>>,
                                std::
                               > InteractQueue;

    Meta(Enable)
    InteractionBay(void);
    ~InteractionBay(void);

    Meta(Enable)
    void OnInitialize(void) override;
    Meta(Enable)
    void OnCollision(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    void Clear(void);

    Meta(Disable)
    InteractionBay::InteractQueue m_distances;

    Meta(Disable)
    Interactable* m_prevInteractable;

} Meta(Enable, WhiteListMethods, DisplayName("InteractionBay"));
