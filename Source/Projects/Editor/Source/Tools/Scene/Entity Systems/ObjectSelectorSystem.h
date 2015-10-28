#pragma once

#include <EntitySystem.h>
#include <Core/Graphics/API/GfxAPI.h>
#include <MouseManager.h>
#include <KeyboardManager.h>
#include <Vec3.h>

class ObjectSelectorSystem : public ursine::ecs::EntitySystem
{
    enum ToolType
    {
        TOOL_TRANSLATION = 0,
        TOOL_SCALE,
        TOOL_ROTATION,
        TOOL_COUNT
    };

    ENTITY_SYSTEM;
public:
    ObjectSelectorSystem(ursine::ecs::World *world);

    ursine::ecs::Entity *GetCurrentFocus();

    //private methods
private:
    void OnInitialize(void) override;
    void OnRemove(void) override;

    //when the mouse is pressed down
    void onMouseDown(EVENT_HANDLER(ursine::MouseManager));

    //when the mouse is moved
    void onMouseMove(EVENT_HANDLER(ursine::MouseManager));

    //when the mouse is released
    void onMouseUp(EVENT_HANDLER(ursine::MouseManager));

    //when the mouse is released
    void onMouseScroll(EVENT_HANDLER(ursine::MouseManager));

    //when a key is pressed
    void onKeyDown(EVENT_HANDLER(ursine::KeyboardManager));

    //when the world updates
    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    //change the offset values of the tool
    void calculateOffset(ursine::Vec2 mousePos);

    //change where tool should be
    void updateToolPosition(ursine::Vec3 pos);

    //move tool to object
    void moveToolToEntity(const ursine::ecs::EntityUniqueID id);

    //given a handle, select an object
    void pickObject(const ursine::ecs::EntityUniqueID id);
    
    //given a handle, unselect an object
    void unpickObject(const ursine::ecs::EntityUniqueID id);

    //set tool to translation
    void setToTranslate(void);
    void updateTranslation(const ursine::SVec3 &mousePos);

    //set tool to scale
    void setToScale(void);
    void updateScale(const ursine::SVec3 &mousePos);

    //set tool to rotation
    void setToRotation(void);
    void updateRotation(const ursine::SVec3 &mousePos);

    //hide tool
    void hideTool( );

    //show tool

    //get mouse position
    ursine::SVec3 GetMousePosition(const ursine::Vec2 mousePos);

    //update out current bases for the current obj
    void updateBases( );

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

    //what tool are we using
    ToolType m_currentTool;

    //where is the base?
    ursine::Vec3 m_baseTranslation;
    ursine::Vec3 m_offset;
    ursine::Vec3 m_baseScale;
    ursine::SQuat m_baseRotation;
    ursine::Vec3 m_baseMousePos;
} Meta(Enable);