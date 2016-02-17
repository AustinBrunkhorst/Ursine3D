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

//////////////////////
////  Predefines  ////
//////////////////////

struct InteractionBay;

//////////////////////
//////////////////////


class InteractionBaySystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM ;

public:
    InteractionBaySystem(ursine::ecs::World* world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(ursine::ecs::Entity* entity) override;
    void Disable(ursine::ecs::Entity* entity) override;

private:

    // update current bay
    void UpdateBay(InteractionBay* bay, const int closestIndex);

    std::unordered_map< ursine::ecs::EntityUniqueID, ursine::ecs::Component::Handle< InteractionBay > > m_interactionBays;
} Meta(Enable, AutoAddEntitySystem);








