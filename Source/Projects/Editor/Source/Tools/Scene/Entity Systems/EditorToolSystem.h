/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ObjectSelectorSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - Matt Yan m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>
#include <MouseManager.h>
#include <KeyboardManager.h>

class EditorToolSystem : public ursine::ecs::EntitySystem
{
	ENTITY_SYSTEM;

public:
	EditorToolSystem(ursine::ecs::World *world);

	ursine::ecs::Entity *GetCurrentFocus(void);

private:
	void OnInitialize(void) override;
	void OnRemove(void) override;

	void onUpdate(EVENT_HANDLER(ursine::ecs::World));

	// When the mouse is pressed down
	void onMouseDown(EVENT_HANDLER(ursine:MouseManager));

	// When the mouse is released
	void onMouseUp(EVENT_HANDLER(ursine::MouseManager));

	// When the mouse is moved
	void onMouseMove(EVENT_HANDLER(ursine::MouseManager));

	// When the mouse scrolls
	void onMouseScroll(EVENT_HANDLER(ursine::MouseManager));

	// When a key is pressed
	void onKeyDown(EVENT_HANDLER(ursine::KeyboardManager));

	// When a key is released
	void onKeyUp(EVENT_HANDLER(ursine::KeyboardManager));

	void onSelectedAdd(EVENT_HANDLER(ursine::ecs::World));

	ursine::MouseManager *m_mouseManager;
	ursine::KeyboardManager *m_keyboardManager;

	// current ID we're locked onto
	ursine::ecs::EntityUniqueID m_currentID;
};