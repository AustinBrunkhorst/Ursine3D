#pragma once

#include <EntitySystem.h>
#include <Core/Graphics/API/GfxAPI.h>
#include <MouseManager.h>
#include <Vec3.h>

class ObjectSelectorSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;
public:
    ObjectSelectorSystem(ursine::ecs::World *world);

    ursine::ecs::Entity *GetCurrentFocus();

    //private methods
private:
    void OnInitialize(void) override;
    void OnRemove(void) override;

    void onMouseDown(EVENT_HANDLER(ursine::MouseManager));

    void onMouseMove(EVENT_HANDLER(ursine::MouseManager));

    void onMouseUp(EVENT_HANDLER(ursine::MouseManager));

    void calculateOffset(ursine::Vec2 mousePos);

    void updateAxis(ursine::Vec3 pos);
    //members
private:
    //visual representations of the 3 axis
    ursine::ecs::Entity *m_xAxis;
    ursine::ecs::Entity *m_yAxis;
    ursine::ecs::Entity *m_zAxis;

    //current ID we're locked onto
    ursine::ecs::EntityUniqueID m_currentID;

    ursine::graphics::GfxAPI *m_graphics;

    //are we dragging right now?
    bool m_dragging;

    //what axis is currently selected?
    int m_axis;

    //where is the base?
    ursine::Vec3 m_basePos;
    ursine::Vec3 m_translateOffset;
} Meta(Enable);