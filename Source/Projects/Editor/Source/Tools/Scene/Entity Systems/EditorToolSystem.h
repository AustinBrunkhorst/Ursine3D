/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ObjectSelectorSystem.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>
#include <MouseManager.h>
#include <KeyboardManager.h>
#include <EditorCameraSystem.h>
#include <EditorTool.h>
#include <SelectTool.h>
#include <DuplicateTool.h>

class EditorToolSystem : public ursine::ecs::EntitySystem
{
	ENTITY_SYSTEM;

public:
	EditorToolSystem(ursine::ecs::World *world);
	~EditorToolSystem(void);

	ursine::ecs::Entity *GetCurrentFocus(void);

    void ClearSelectedEntities(void);

private:
	void OnAfterLoad(void) override;
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
	void onSelectedRemoved(EVENT_HANDLER(ursine::ecs::World));

	ursine::MouseManager *m_mouseManager;
	ursine::KeyboardManager *m_keyboardManager;

	EditorCameraSystem *m_editorCameraSystem;

	// current ID we're locked onto
	ursine::ecs::EntityUniqueID m_currentSelected;

	std::unordered_map<ursine::KeyboardKey, EditorTool*> m_tools;

	EditorTool *m_currentTool;

	SelectTool *m_selectTool;
	DuplicateTool *m_dupTool;
} Meta(Enable, AutoAddEntitySystem);