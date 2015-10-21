#pragma once

#include <EntitySystem.h>
#include <Core/Graphics/API/GfxAPI.h>

class ObjectSelectorSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;
public:
    ObjectSelectorSystem(ursine::ecs::World *world);

    //private methods
private:
    void OnInitialize(void) override;
    void OnRemove(void) override;
    void onRenderHook(EVENT_HANDLER(ursine::ecs::RenderSystem));

    //members
private:
    ursine::graphics::GfxHND m_xAxis;
    ursine::graphics::GfxHND m_yAxis;
    ursine::graphics::GfxHND m_zAxis;

    ursine::graphics::GfxAPI *m_graphics;
};