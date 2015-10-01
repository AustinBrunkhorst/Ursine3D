#pragma once

#include <EntitySystem.h>
#include <RenderSystem.h>

class GridRenderer : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;
public:
    GridRenderer(ursine::ecs::World *world);

private:
    ursine::GfxAPI *m_graphics;
    ursine::ecs::RenderSystem *m_renderSystem;

    void OnInitialize(void) override;
    void OnRemove(void) override;

    void onRenderHook(EVENT_HANDLER(ursine::ecs::RenderSystem));
} Meta(Enable);