/* ----------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorTool.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <KeyboardManager.h>
#include <MouseManager.h>

class Editor;

class EditorTool
{
public:
	EditorTool(Editor *editor);

	virtual void OnEnable(void) { }
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
};
