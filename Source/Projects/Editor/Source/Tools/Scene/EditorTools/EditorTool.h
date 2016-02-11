/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorTool.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <KeyboardManager.h>
#include <MouseManager.h>

#include "Editor.h"

class EditorTool
{
public:
	EditorTool(Editor *editor, ursine::ecs::World *world);

	virtual void OnEnable(ursine::ecs::EntityUniqueID selected) { }
	virtual void OnDisable(void) { }

	virtual void OnSelect(ursine::ecs::Entity *selected) { }
	virtual void OnDeselect(ursine::ecs::Entity *deselected) { }

	virtual void OnUpdate(ursine::KeyboardManager *kManager, ursine::MouseManager *mManager) { }

	virtual void OnMouseDown(const ursine::MouseButtonArgs &args) { }
	virtual void OnMouseUp(const ursine::MouseButtonArgs &args) { }

	virtual void OnMouseMove(const ursine::MouseMoveArgs &args) { }

	virtual void OnMouseScroll(const ursine::MouseScrollArgs &args) { }

	virtual void OnKeyDown(const ursine::KeyboardKeyArgs &args) { }
	virtual void OnKeyUp(const ursine::KeyboardKeyArgs &args) { }

protected:
	Editor *m_editor;
	ursine::ecs::World *m_world;
};
