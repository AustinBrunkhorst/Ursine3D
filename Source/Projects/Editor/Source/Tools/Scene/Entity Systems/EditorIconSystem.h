/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorIconSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>

class EditorIcon;

namespace ursine
{
    namespace ecs
    {
        enum class LightType;
    }
}

class EditorIconSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;
public:
    EditorIconSystem(ursine::ecs::World *world);

private:
    ursine::resources::ResourceManager &m_toolResources;

    void OnSceneReady(ursine::Scene *scene) override;
    void OnRemove(void) override;

    void onIconAdd(EVENT_HANDLER(ursine::ecs::World));

    void onLightTypeChange(EVENT_HANDLER(ursine::ecs::World));

    void setLightIcon(ursine::ecs::LightType type, EditorIcon *icon);

} Meta(Enable, AutoAddEntitySystem);