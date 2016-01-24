/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RotateTool.cpp
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "RotateTool.h"

#include <EditorConfig.h>
#include <SystemManager.h>
#include <CameraComponent.h>
#include <Model3DComponent.h>

using namespace ursine;
using namespace ecs;

RotateTool::RotateTool(Editor *editor)
	: EditorTool( editor )
	, m_gizmo( nullptr )
	, m_selected( -1 )
	, m_dragging( false )
	, m_snapping( false )
	, m_local( false )
	, m_deleteGizmo( false )
{
	m_graphics = GetCoreSystem( graphics::GfxAPI );
	m_world = m_editor->GetProject( )->GetScene( )->GetWorld( );
	m_editorCameraSystem = m_world->GetEntitySystem( EditorCameraSystem );
}

void RotateTool::OnEnable(EntityUniqueID selected)
{
	m_selected = selected;

	if (m_selected != -1)
		enableAxis( );
}

void RotateTool::OnDisable(void)
{
	m_selected = -1;
	m_dragging = false;
	m_snapping = false;
	
	disableAxis( );
}

void RotateTool::OnSelect(Entity* entity)
{
	auto prevSelected = m_gizmo != nullptr;
	
	m_selected = entity->GetUniqueID( );

	if (!prevSelected)
		enableAxis( );

	m_deleteGizmo = false;
}

void RotateTool::OnDeselect(Entity* entity)
{
	m_selected = -1;
	
	m_deleteGizmo = true;
}

void RotateTool::OnMouseDown(const MouseButtonArgs& args)
{
	// get the current entity ID the mouse is over
	auto newID = m_graphics->GetMousedOverID( );
	auto entity = m_world->GetEntityUnique( newID );

	if (!entity || m_altDown)
		return;

	auto entityTrans = entity->GetTransform( );

	auto rootName = entityTrans->GetRoot( )->GetOwner( )->GetName( );

	// if we're clicking on ourselves, set the dragging flag,
	// and the vector we're dragging on
	if (rootName == "RotationGizmo")
	{
		m_dragging = true;

		// Get the selected entity
		auto selected = m_world->GetEntityUnique( m_selected );

		// Get the gizmo's name (the models are under the parent named the axis' name)
		auto name = entityTrans->GetParent( )->GetOwner( )->GetName( );

		if (name == "xAxis")
			setDirectionVectors( SVec3::UnitX( ), selected );
		else if (name == "yAxis")
			setDirectionVectors( SVec3::UnitY( ), selected );
		else if (name == "zAxis")
			setDirectionVectors( SVec3::UnitZ( ), selected );
	}
}

void RotateTool::OnMouseUp(const MouseButtonArgs& args)
{
	if (m_dragging)
		m_dragging = false;
}

void RotateTool::OnMouseMove(const MouseMoveArgs& args)
{
	// m_worldDir is the plane normal.  Get distance to rotate about it

	if (m_dragging && m_selected != -1)
	{
		// Project the delta vector onto the screen direction vector
		auto b = args.positionDelta;
		auto &a = m_screenDir;

		// invert the vector (because reasons)
		b = -b;

		float dot = a.Dot( b );
		auto proj = ( dot / b.LengthSquared( ) ) * b;
		auto dist = proj.Length( );
		auto selected = m_world->GetEntityUnique( m_selected )->GetTransform( );

		if (dot < 0.0f)
			dist = -dist;

		dist *= 5.0f;

		// Snap object, move tool
		/*if (m_snapping)
		{
			m_accum += dist;

			dist = 0.0f;

			if (abs( m_accum ) > 45.0f)
			{
				dist = 45.0f * (m_accum < 0.0f ? -1.0f : 1.0f);
				m_accum = 0.0f;
			}
		}*/

		selected->SetWorldRotation( selected->GetWorldRotation( ) * SQuat( dist, m_worldDir ) );
	}
}

void RotateTool::OnKeyDown(const KeyboardKeyArgs& args)
{
	if (args.key == KEY_SPACE)
		m_local = !m_local;

	if (args.key == KEY_LCONTROL)
		m_snapping = true;

	if (args.key == KEY_LMENU)
		m_dragging = false;
}

void RotateTool::OnKeyUp(const KeyboardKeyArgs& args)
{
	if (args.key == KEY_CONTROL)
		m_snapping = false;
}

void RotateTool::OnUpdate(KeyboardManager* kManager, MouseManager* mManager)
{
	if (m_deleteGizmo)
	{
		disableAxis( );
		m_deleteGizmo = false;
	}

	if (kManager->IsDown( KEY_LMENU ))
		m_altDown = true;
	else
		m_altDown = false;

	updateAxis( );
}

void RotateTool::setDirectionVectors(const SVec3& basisVector, Entity* selected)
{
	
	auto computeVec = selected->GetTransform( )->GetWorldRotation( ) * basisVector;
	
	m_worldDir = basisVector;

	// Set the screen direction
	auto cam = m_editorCameraSystem->GetEditorCamera( );
	auto worldP = selected->GetTransform( )->GetWorldPosition( );
	auto p1 = cam->WorldToScreen( worldP );
	auto p2 = cam->WorldToScreen( worldP + computeVec );
	
	auto temp = p2 - p1;
	temp.Normalize( );

	// Take the vector that is perpendicular to this vector
	m_screenDir.X( ) = temp.Y( );
	m_screenDir.Y( ) = temp.X( );
}

void RotateTool::enableAxis(void)
{
	m_gizmo = m_world->CreateEntityFromArchetype( 
		EDITOR_ARCHETYPE_PATH "EditorTools/RotationGizmo.uatype",
		"RotationGizmo" 
	);

	setEntitySerializationToggle( false, m_gizmo );
	
	// Make it invisible in the hiearchy
	m_gizmo->SetVisibleInEditor( false );

	// Make all models draw over everything
	for (auto &model : m_gizmo->GetComponentsInChildren<ecs::Model3D>( ))
	{
		model->SetOverdraw( true );
		model->SetMaterialData( 4, 0, 0 );
	}
}

void RotateTool::disableAxis(void)
{
	if (m_gizmo)
	{
		m_gizmo->Delete( );
		
		// Clear the deletion queue if the scene is paused
		EditorClearDeletionQueue( );
	}

	m_gizmo = nullptr;
}

void RotateTool::updateAxis(void)
{
	if (m_selected == -1 || m_gizmo == nullptr)
		return;

	// update the size of the gizmo
	auto gizTrans = m_gizmo->GetTransform( );

	gizTrans->SetWorldScale( SVec3( m_editorCameraSystem->GetCamZoom( ) * 0.03f ) );

	auto selected = m_world->GetEntityUnique( m_selected );
	auto selTrans = selected->GetTransform( );

	gizTrans->SetWorldPosition( selTrans->GetWorldPosition( ) );

	if (m_local)
		gizTrans->SetWorldRotation( selTrans->GetWorldRotation( ) );
	else
		gizTrans->SetLocalRotation( SQuat::Identity( ) );
}

void RotateTool::setEntitySerializationToggle(bool toggle, Entity* entity)
{
	for (auto *child : entity->GetTransform( )->GetChildren( ))
	{
		setEntitySerializationToggle( toggle, child->GetOwner( ) );
	}

	entity->EnableSerialization( toggle );
}
