/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DuplicateTool.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "EditorTool.h"
#include "EditorCameraSystem.h"
#include "Plane.h"
#include "DebugSystem.h"

class DuplicateTool : public EditorTool
{
public:
	DuplicateTool(Editor *editor, ursine::ecs::World *world);

	void OnEnable(ursine::ecs::EntityUniqueID selected) override;
	void OnDisable(void) override;

	void OnSelect(ursine::ecs::Entity *entity) override;
	void OnDeselect(ursine::ecs::Entity *entity) override;

	void OnMouseDown(const ursine::MouseButtonArgs &args) override;

	void OnKeyDown(const ursine::KeyboardKeyArgs &args) override;
	void OnKeyUp(const ursine::KeyboardKeyArgs &args) override;

	void OnUpdate(ursine::KeyboardManager *kManager, ursine::MouseManager *mManager) override;

private:
	ursine::graphics::GfxAPI *m_graphics;

	EditorCameraSystem *m_editorCameraSystem;
	
	ursine::ecs::DebugSystem *m_drawer;

	// visual representation of the 3 axis
	ursine::ecs::Entity *m_gizmo;

	ursine::ecs::EntityUniqueID m_selected;

	enum PlaneType
	{
		PLANE_XZ,
		PLANE_ZY,
		PLANE_XY,
		PLANE_NUM
	};

	ursine::Plane m_plane;
	PlaneType m_planeType;

	// Whether the user is holding down the snap to grid key or not
	bool m_snapping;

	// This flag is needed to solve a circular locking problem with clear deletion queue
	bool m_deleteGizmo;

	// Whether the user is using alt
	bool m_altDown;

	bool m_origin;

	void enableGizmo(void);
	void disableGizmo(void);
	void updateGizmo(ursine::MouseManager *mManager);

	void debugDraw(void);

	void setEntitySerializationToggle(bool toggle, ursine::ecs::Entity* entity);
};
