#pragma once

#include "CoreSystem.h"
#include "Window.h"
#include "UIView.h"
#include "PhysicsManager.h"

class PhysicsTest : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
	PhysicsTest(void);
    ~PhysicsTest(void);

    Meta(Disable)
    void OnInitialize(void) override;

    Meta(Disable)
    void OnRemove(void) override;
    
private:
    ursine::Window *m_mainWindow;

    CefRefPtr<ursine::UIView> m_ui;

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));

    void onMouseScroll(EVENT_HANDLER(MouseManager));

    void UpdateCamera(float dt);
    void UpdateCamera_Mouse(float dt);
    void UpdateCamera_Keys(float dt);

    void RenderGrid();

    void initGraphics(void);
    void initPhysics(void);

    // Graphics Related
    ursine::GfxAPI *m_gfx;

    GFXHND m_viewport;
    GFXHND m_viewport2;
    GFXHND m_camera;
    GFXHND m_camera2;
    GFXHND m_cube;
    GFXHND m_billboard;
    // static GFXHND wirePrimitive;
    GFXHND m_floor;
    GFXHND m_light;
    GFXHND m_light2;
    GFXHND m_directLight;

    GFXHND m_primitive;

    GFXHND m_spheres[10][10];

    GFXHND m_lights[12];

    //tweens

    ursine::SVec3 m_camPos;
    float m_camZoom;
    float m_phi;

    ursine::SQuat m_currentQuat;

	// Physics Related
	ursine::PhysicsManager *m_physics;

} Meta(Enable);