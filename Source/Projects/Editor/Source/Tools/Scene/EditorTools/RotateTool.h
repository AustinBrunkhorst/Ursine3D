/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RotateTool.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "EditorTool.h"
#include "EditorCameraSystem.h"
#include <Model3DComponent.h>

class RotateTool : public EditorTool
{
public:
	RotateTool(Editor *editor, ursine::ecs::World *world);

	void OnEnable(ursine::ecs::EntityUniqueID selected) override;
	void OnDisable(void) override;

	void OnSelect(ursine::ecs::Entity *entity) override;
	void OnDeselect(ursine::ecs::Entity *entity) override;

	void OnMouseDown(const ursine::MouseButtonArgs &args) override;
	void OnMouseUp(const ursine::MouseButtonArgs &args) override;

	void OnMouseMove(const ursine::MouseMoveArgs &args) override;

	void OnKeyDown(const ursine::KeyboardKeyArgs &args) override;
	void OnKeyUp(const ursine::KeyboardKeyArgs &args) override;

	void OnUpdate(ursine::KeyboardManager *kManager, ursine::MouseManager *mManager) override;

private:
	ursine::graphics::GfxAPI *m_graphics;

	EditorCameraSystem *m_editorCameraSystem;

	// visual representation of the 3 axis
	ursine::ecs::Entity *m_gizmo;

	ursine::ecs::EntityUniqueID m_selected;

	// Whether or not the user is clicking and dragging on the gizmo
	bool m_dragging;

	// Whether or not the user is hovering over an axis
	bool m_hovering;

	ursine::ecs::Component::Handle<ursine::ecs::Model3D> m_axis;
	ursine::Color m_axisOrigColor;

	// Whether the user is holding down the snap to grid key or not
	bool m_snapping;

	// A flag letting us know if we're using local space transformations or not
	bool m_local;

	bool m_altDown;

	// This flag is needed to solve a circular locking problem with clear deletion queue
	bool m_deleteGizmo;

	// This accumulation value is so that we can snap to a certain rotation
	float m_accum;

	// The direction we're dragging in world coordinates
	ursine::SVec3 m_worldDir;

	// The world direction projected onto screen coordinates
	ursine::Vec2 m_screenDir;

	// Helper for setting world and screen directions
	void setDirectionVectors(const ursine::SVec3 &basisVector, ursine::ecs::Entity *selected);

	// Helpers for handling the spawned translation archetype
	void enableAxis(void);
	void disableAxis(void);
	void updateAxis(void);

	void updateHoverAxis(void);
	void disableHover(void);

	// recursive helper function
	void setEntitySerializationToggle(bool toggle, ursine::ecs::Entity *entity);
};
