#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TrailSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>

//////////////////////
////  Predefines  ////
//////////////////////

class TrailComponent;

namespace ursine
{
    namespace ecs
    {
        class Transform;
        class ParticleEmitter;

    } // ecs namespace
} // ursine namespace

//////////////////////
//////////////////////


class TrailSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM ;

public:
    TrailSystem(ursine::ecs::World* world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(ursine::ecs::Entity* entity) override;
    void Disable(ursine::ecs::Entity* entity) override;

private:

    void TrailSystem::UpdateTrail(ursine::ecs::EntityUniqueID id, TrailComponent* const trail, float dt);

    std::unordered_map< ursine::ecs::EntityUniqueID, TrailComponent* > m_trails;
    std::unordered_map< ursine::ecs::EntityUniqueID, ursine::ecs::Transform* > m_transforms;
    std::unordered_map< ursine::ecs::EntityUniqueID, ursine::ecs::ParticleEmitter* > m_particleEmitter;

} Meta(Enable);



