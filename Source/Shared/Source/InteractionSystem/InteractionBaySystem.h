#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractionBaySystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>
#include <Component.h>
#include "InteractionBayComponent.h"

//////////////////////
////  Predefines  ////
//////////////////////

struct InteractionBay;
struct Interactable;
//////////////////////
//////////////////////


class InteractionBaySystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    InteractionBaySystem(ursine::ecs::World *world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;

    void Enable(const ursine::ecs::EntityHandle &entity) override;
    void Disable(const ursine::ecs::EntityHandle &entity) override;

private:

    typedef InteractionBay::PrevIter PrevIter;
    typedef InteractionBayUtils::InteractInfo InteractInfo;

    // update current bay
    void UpdateBay(InteractionBay* bay);
    bool InteractUpdate(InteractionBay* bay, const InteractInfo& currInteractable, PrevIter& prevIt, PrevIter& prevEnd);

    std::unordered_map<ursine::ecs::EntityHandle, InteractionBay*> m_interactionBays;

} Meta(Enable, AutoAddEntitySystem);
