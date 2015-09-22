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

	void initGraphics(void);
	void initPhysics(void);

	// Graphics Related
	ursine::GfxAPI *m_gfx;

	GFXHND m_viewport;
	GFXHND m_camera;
	GFXHND m_cube;
	// static GFXHND wirePrimitive;
	GFXHND m_floor;
	GFXHND m_light;
	GFXHND m_light2;

	// Physics Related
	// ursine::PhysicsManager *m_physics;

} Meta(Enable);