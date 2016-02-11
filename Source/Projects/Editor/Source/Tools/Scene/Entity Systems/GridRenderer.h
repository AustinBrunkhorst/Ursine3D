/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GridRenderer.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>
#include <RenderSystem.h>
#include <Core/Graphics/API/GfxAPI.h>

class GridRenderer : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;
public:
    GridRenderer(ursine::ecs::World *world);

private:
    ursine::graphics::GfxAPI *m_graphics;
    ursine::ecs::RenderSystem *m_renderSystem;

    void OnInitialize(void) override;
    void OnRemove(void) override;

    void onRenderHook(EVENT_HANDLER(ursine::ecs::RenderSystem));
} Meta(Enable, AutoAddEntitySystem);