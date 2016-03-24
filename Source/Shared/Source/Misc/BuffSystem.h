#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** � 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BuffSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>


//////////////////////
////  Predefines  ////
//////////////////////

class BuffComponent;

//////////////////////
//////////////////////


class BuffSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM ;

public:
    BuffSystem(ursine::ecs::World* world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(const ursine::ecs::EntityHandle &entity) override;
    void Disable(const ursine::ecs::EntityHandle &entity) override;

private:

    // entity's components changed
    void OnComponentAdded(EVENT_HANDLER(World));

    // entity removed
    void OnEntityRemoved(EVENT_HANDLER(World));

    std::unordered_map< ursine::ecs::EntityHandle, BuffComponent* > m_buffComps;
} Meta(Enable);
