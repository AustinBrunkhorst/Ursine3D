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

namespace InteractionBayUtils
{
    Meta(Disable)
    typedef std::pair<float, Interactable*> InteractInfo;


    Meta(Disable)
    bool Compare(const InteractInfo& lhs, const InteractInfo& rhs);
} // InteractionBayUtils namespace

struct InteractionBay : ursine::ecs::Component
{
private:
    NATIVE_COMPONENT;

    Meta(Disable)
    typedef std::set< Interactable* > PrevIteractables;

    Meta(Disable)
        typedef std::set< Interactable* > CurrInteractables;

public:
    Meta(Disable)
    typedef InteractionBayUtils::InteractInfo InteractInfo;

    Meta(Disable)
    typedef PrevIteractables::iterator PrevIter;

    Meta(Disable)
    typedef std::priority_queue<InteractInfo,
                                std::vector<std::pair<float, Interactable*>>,
                                std::function<bool(InteractInfo&, InteractInfo&)>
                               > InteractQueue;

    Meta(Enable)
    InteractionBay(void);
    ~InteractionBay(void);

    Meta(Enable)
    void OnInitialize(void) override;
    Meta(Enable)
    void OnCollision(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    void AddInteractable(float distSquared, Interactable* interactable);

    void Update(void);

    void Clear(void);

    Meta(Disable)
    InteractQueue m_interactQueue;

    Meta(Disable)
    PrevIteractables m_prevInteractables;

    Meta(Disable)
    CurrInteractables m_currInteractables;

} Meta(Enable, WhiteListMethods, DisplayName("InteractionBay"));
